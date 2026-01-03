import { useState } from "react";
import "./App.css";
import MutexGraph from "./MutexGraph";
import { Shuffle } from "lucide-react";

function App() {

  const [showCFG, setShowCFG] = useState<any>(true);
  const [justAddedData, setJustAddedData] = useState<any>(true);

  return (
    <>
      <div className="flex flex-col p-4 h-screen">
      <div className="flex mb-6 justify-between items-center">
          <div>
            <h1 className="text-2xl font-bold">
              {!showCFG ? "Control Flow Graph" : "Mutex Dependency Graph"}
            </h1>
            <p className="text-gray-600">Visualization component</p>
          </div>
          <div>
            <button
              className="flex items-center rounded-md shadow-sm gap-2 bg-sky-50 border border-1 border-sky-200 hover:bg-sky-100 px-4 py-2 text-sky-800 font-medium text-sm"
              onClick={() => {
                setShowCFG(!showCFG);
                setJustAddedData(false);
              }}
            >
              <Shuffle className="w-4 h-4 text-sky-800" />
              Switch Graph
            </button>
          </div>
        </div>
        <div className="h-full">
          <MutexGraph buttonState={showCFG} setButtonState={setShowCFG} justAddedData={justAddedData} setJustAddedData={setJustAddedData} />
        </div>
      </div>
    </>
  );
}

export default App;
