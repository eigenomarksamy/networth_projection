window.addEventListener('DOMContentLoaded', () => {
    // Access the exported functions from the WebAssembly module
    const myFunction = Module.cwrap('myFunction', null, []);
    
    // Call the C++ function
    myFunction();
  });
  