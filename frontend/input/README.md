# Input Directory
 
This input directory contains the generated JSON file from the analyzer and the file that the frontend will pull in to the visualize to the user. 

You can think of this folder as a bridge between both sides of the project. Do not modify the contents explicitly. Only write to it with the results from the analyzer and read from it in the frontend.  

File outputs from the analyzer should be written to `input/input.json` and subsequent reads from that file should be read as input from the frontend. As part of a demonstration of the visualizationt components, we provide a default `input.json` file for demonstration purposes. 

## Notes

You must use the Docker container for the output of the analyzer to be automatically placed into this `input` folder (within the mounted directory). Otherwise, if setting up the development environment manually, you must copy and paste the result into the `input.json` file, set the `-o` flag in the program to the correct file and directory (i.e. `frontend/input/input.json`), or upload the JSON file directly on the user interface that the Vite server runs.  