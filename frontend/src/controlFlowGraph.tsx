import { useEffect, useState } from "react";
import Mermaid from "react-mermaid2"

interface ControlFlowGraphProps {
    graphData: any;
    justAddedData: any;
}

export const ControlFlowGraph: React.FC<ControlFlowGraphProps> = ({ graphData, justAddedData }) => {
    const [graph, setGraph] = useState<any>("");

    useEffect(() => {
        renderGraph();
    }, [graphData]);

    const renderGraph = () => {
        let initString = "graph TD;"
        console.log("GraphData dasdsa", graphData);
        if (graphData?.CFG?.edges) {
            for (let i = 0; i < graphData?.CFG?.edges?.length; i++) {
                console.log("ControlFlowGraphData", graphData)
                if (graphData.CFG.edges[i].type === "true") {
                    console.log("check graph Data", graphData.CFG.edges[i]);
                    if (typeof graphData.CFG.edges[i].from === "string") {
                        const code1 = `${graphData.CFG.edges[i].from}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].from).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].to}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].to).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        console.log("changes", code1);
                        initString += ` ${graphData.CFG.edges[i].from}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].to}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].from}-- true --->${graphData.CFG.edges[i].to};`
                    } else {
                        const code1 = `${graphData.CFG.edges[i].source}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].source).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].target}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].target).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        console.log("changes", code1);
                        initString += ` ${graphData.CFG.edges[i].source}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].target}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].source}-- true --->${graphData.CFG.edges[i].target};`
                    }
                } else if (graphData.CFG.edges[i].type === "false") {
                    if (typeof graphData.CFG.edges[i].from === "string") {
                        const code1 = `${graphData.CFG.edges[i].from}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].from).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].to}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].to).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        initString += ` ${graphData.CFG.edges[i].from}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].to}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].from}-- false --->${graphData.CFG.edges[i].to};`
                    } else {
                        const code1 = `${graphData.CFG.edges[i].source}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].source).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].target}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].target).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        console.log("changes", code1);
                        initString += ` ${graphData.CFG.edges[i].source}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].target}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].source}-- false --->${graphData.CFG.edges[i].target};`
                    }
                } else {
                    if (typeof graphData.CFG.edges[i].from === "string") {
                        const code1 = `${graphData.CFG.edges[i].from}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].from).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].to}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].to).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        initString += ` ${graphData.CFG.edges[i].from}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].to}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].from}-->${graphData.CFG.edges[i].to};`
                    } else {
                        const code1 = `${graphData.CFG.edges[i].source}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].source).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        const code2 = `${graphData.CFG.edges[i].target}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes.find((e: any) => e.id === graphData.CFG.edges[i].target).code.join("<br/>").replaceAll("\"", "\'")}</code>`
                        console.log("changes", code1);
                        initString += ` ${graphData.CFG.edges[i].source}["${code1}"]; `
                        initString += ` ${graphData.CFG.edges[i].target}["${code2}"]; `
                        initString += ` ${graphData.CFG.edges[i].source}-->${graphData.CFG.edges[i].target};`
                    }
                }
            }
        } else {
            for (let i = 0; i < graphData?.CFG?.nodes.length; i++) {
                const code = `${graphData.CFG.nodes[i].id}<br/><br/><b>code</b><br/><code>${graphData.CFG.nodes[i].code.join("<br/>").replaceAll("\"", "\'")}</code>`
                initString += ` ${graphData.CFG.nodes[i].id}["${code}"]; `
                initString += ` ${graphData.CFG.nodes[i].id};`
            }
        }
        console.log(graphData)
        console.log("initString", initString);
        setGraph(initString);
        // return <Mermaid chart={initString} config={{
        //     theme: "default"
        // }} />
    }
    
    return (
        <div className="w-full h-full bg-white flex justify-center items-center">
            {!justAddedData &&
            <div className="w-4/5 h-4/5">
            <Mermaid chart={graph} config={{
            theme: "default"
        }}/>
        </div>
    }
        {justAddedData &&
        <div>
            <h2 className="text-gray-800">Click Switch Graph to See New Deadlock Graph!</h2>
        </div>
        }
        </div>
    )
}