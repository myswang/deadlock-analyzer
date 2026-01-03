import {
  AlertCircle,
  ArrowRightCircle,
  CircleIcon,
  Code,
  FileIcon,
  GitPullRequestClosed,
  Info,
  Lock,
  MousePointerClick,
  UploadCloud,
} from "lucide-react";

interface InfoPanelProps {
  selectedElement: any;
  type: "node" | "edge" | null;
  currentFileName: string;
  graphData: any;
}

/**
 * InfoPanel contains code adapted with assistance from ChatGPT and Claude during April 2025.
 */
const InfoPanel: React.FC<InfoPanelProps> = ({
  selectedElement,
  type,
  currentFileName,
  // @ts-ignore
  graphData,
}) => {
  const getSourceCode = (lineNum: number): string => {
    if (!graphData?.source) return "No source code available";

    const lines = graphData.source.split("\n");
    const start = Math.max(1, lineNum - 2);
    const end = Math.min(lines.length, lineNum + 2);

    let snippet = "";
    for (let i = start - 1; i < end; i++) {
      const lineNum = i + 1;
      const isHighlighted = lineNum === lineNum;
      snippet += `${lineNum}${isHighlighted ? " →" : "  "} ${lines[i]}\n`;
    }
    return snippet;
  };

  if (!selectedElement) {
    return (
      <div className="space-y-4">
        <div className="bg-white p-4 rounded border border-gray-200 text-gray-500">
          <h3 className="font-medium text-gray-900 mb-2">Graph details</h3>
          <div className="flex items-center gap-2 mb-4">
            <MousePointerClick className="w-4 h-4" />
            <div className="">Try clicking on node or edge</div>
          </div>
          <div className="pt-3 border-t border-gray-100 space-y-3">
            <div className="text-gray-700 font-medium">Graph interactions:</div>
            <div className="flex gap-2">
              <CircleIcon className="w-4 h-4 text-green-500 mt-0.5" />
              <div className="text-sm">
                <span className="font-medium">Click on a node</span> to see its
                type (mutex/process), relationships with other nodes, and
                deadlock details for mutex resources
              </div>
            </div>
            <div className="flex gap-2">
              <CircleIcon className="w-4 h-4 text-green-500 mt-0.5" />
              <div className="text-sm">
                <span className="font-medium">Click on a edge</span> to view
                detailed edge relationship information and a snapshot of
                relevant source code.
              </div>
            </div>
          </div>
          <div className="mb-4 mt-4 pt-2 border-t border-gray-100">
            <div className="text-gray-500 mb-1">Current file:</div>
            <div className="flex items-center gap-2 bg-gray-50 p-2 rounded">
              <FileIcon className="text-blue-500 w-4 h-4" />
              <div className="text-gray-900">{currentFileName}</div>
            </div>
          </div>
          <div className="bg-blue-50 border border-dashed border-blue-300 p-3 rounded">
            <div className="flex items-center gap-2 text-blue-700">
              <UploadCloud className="w-4 h-4" />
              <div className="font-medium">Drag and drop supported</div>
            </div>
            <div className="text-blue-600 text-xs mt-1">
              Drop a JSON file generated from the static analyzer onto the graph
              to visualize
            </div>
          </div>
        </div>
      </div>
    );
  }
  if (type === "node") {
    const nodeType = selectedElement.type;
    const nodeId = selectedElement.id;

    const relatedEdges =
      graphData?.edges?.filter(
        (edge: any) =>
          edge.source === nodeId ||
          edge.source?.id === nodeId ||
          edge.target === nodeId ||
          edge.target?.id === nodeId
      ) || [];
    return (
      <div className="bg-white rounded border border-gray-200 h-full overflow-hidden flex flex-col">
        <div className="bg-gray-50 p-4 border-b border-gray-100">
          <div className="text-gray-800 font-semibold">
            {nodeType === "resource" ? "Mutex Resource" : "Process"} Details
          </div>
        </div>
        <div className="p-4">
          <div className="flex items-center gap-3">
            <div
              className={`w-8 h-8 flex items-center justify-center rounded-full ${
                nodeType === "resource"
                  ? "bg-purple-100 text-purple-600"
                  : "bg-green-100 text-green-600"
              }`}
            >
              {nodeType === "resource" ? (
                <Lock className="w-4 h-4" />
              ) : (
                <Code className="w-4 h-4" />
              )}
            </div>
            <div className="">
              <div className="text-gray-800 font-medium">{nodeId}</div>
              <div className="text-gray-500 text-sm">{nodeType}</div>
            </div>
          </div>
          {relatedEdges.length > 0 && (
            <div className="mt-4 pt-4 border-t border-gray-100">
              <div className="text-gray-700 text-sm">Relationships:</div>
              <div className="space-y-2">
                {relatedEdges.map((edge: any, index: number) => {
                  const isSource =
                    edge.source === nodeId || edge.source?.id === nodeId;
                  const otherNode = isSource
                    ? edge.target?.id || edge.target
                    : edge.source?.id || edge.source;

                  return (
                    <div
                      key={index}
                      className="bg-gray-50 border border-gray-100 text-sm p-2 space-y-1"
                    >
                      <div className="flex items-center gap-1">
                        <span className="font-medium">{nodeId}</span>
                        <ArrowRightCircle className="w-3 h-3 text-gray-400" />
                        <span className="font-medium">{otherNode}</span>
                      </div>
                      <div className="text-gray-500">
                        {edge.type === "holds"
                          ? "Holds lock"
                          : "Waits for lock"}
                        {edge.function && ` in ${edge.function}()`}
                        {edge.line && ` at line ${edge.line}`}
                      </div>
                    </div>
                  );
                })}
              </div>
            </div>
          )}
          {nodeType === "resource" && graphData?.cycles && (
            <div className="mt-4 pt-4 border-t border-gray-100">
              <div className="text-gray-700 text-sm font-medium mb-2">
                Deadlock Details:
              </div>
              <div className="space-y-2">
                {graphData.cycles
                  .filter((cycle: string) => cycle.includes(nodeId))
                  .map((cycle: string, index: number) => (
                    <div
                      key={index}
                      className="bg-red-50 text-red-700 p-2 border border-red-100 text-sm"
                    >
                      <div className="flex items-center gap-2 mb-1">
                        <GitPullRequestClosed className="w-4 h-4 text-red-500" />
                        <span className="font-medium">Cycle {index + 1}</span>
                      </div>
                      <div>{cycle}</div>
                    </div>
                  ))}
              </div>
            </div>
          )}
        </div>
      </div>
    );
  }

  if (type === "edge") {
    const sourceNode = selectedElement.source?.id || selectedElement.source;
    const targetNode = selectedElement.target?.id || selectedElement.target;
    const relationshipType = selectedElement.type;
    const functionName = selectedElement.function;
    const lineNumber = selectedElement.line;
    const isCyclic = selectedElement.cycle > 0;

    return (
      <div className="bg-white rounded border border-gray-200 h-full overflow-hidden flex flex-col">
        <div className="bg-gray-50 p-4 border-b border-gray-100">
          <div className="text-gray-800 font-semibold">Edge Details</div>
        </div>
        <div className="p-4 flex-1">
          <div
            className={`p-3 ${
              isCyclic
                ? "bg-orange-50 border border-orange-100"
                : "bg-green-50 border border-green-100"
            }`}
          >
            <div className="flex items-center justify-between">
              <div className="text-sm font-medium">Edge Type: </div>
              <div
                className={`text-sm px-2 py-1 rounded  ${
                  relationshipType === "holds"
                    ? "bg-stone-200 text-stone-800"
                    : "bg-rose-100 text-rose-700"
                }`}
              >
                {relationshipType === "holds" ? "Holds lock" : "Waits for lock"}
              </div>
            </div>
            <div className="text-sm flex items-center gap-2 mt-3">
              <div className="text-gray-800 font-medium">{sourceNode}</div>
              <ArrowRightCircle className="w-4 h-4 text-gray-400" />
              <div className="text-gray-800 font-medium">{targetNode}</div>
            </div>
            {isCyclic && (
              <div className="text-red-600 gap-1 mt-3 text-sm flex items-center">
                <AlertCircle className="w-4 h-4" />
                <span>Part of deadlock cycle {selectedElement.cycle}</span>
              </div>
            )}
          </div>
          <div className="mt-4 pt-4 border-t border-gray-100 text-sm">
            <div className="text-gray-700 font-medium mb-2">Code Snapshot:</div>
            <div className="space-y-1">
              <div className="flex justify-between">
                <span className="text-gray-600">Function:</span>
                <span className="bg-gray-100 font-mono px-2 rounded">
                  {functionName}()
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-600">Line:</span>
                <span className="bg-gray-100 font-mono px-2 rounded">
                  {lineNumber}
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-600">Entry Point:</span>
                <span className="bg-gray-100 font-mono px-2 rounded">
                  {selectedElement.entryPt}
                </span>
              </div>
            </div>
          </div>
          {lineNumber && (
            <div className="mt-4 pt-4 border-t border-gray-100">
              <div className="flex gap-2 items-center mb-2">
                <div className="text-gray-700 text-sm font-medium">
                  Source Code:
                </div>
                <span title="Snippet of source program where the wait-for dependency occurs">
                  <Info className="w-3 h-3 text-gray-300" />
                </span>
              </div>
              <div className="bg-gray-50 border border-gray-200">
                <pre className="text-xs p-2 font-mono whitespace-pre overflow-y-auto">
                  {getSourceCode(lineNumber)}
                </pre>
              </div>
            </div>
          )}
        </div>
      </div>
    );
  }
};

export default InfoPanel;
