window.onload = function() {
  var userSelect = document.getElementById("userSelect");
  userSelect.addEventListener("change", handleSelection);
};

function handleSelection() {
  var userSelect = document.getElementById("userSelect");
  var selectedOption = userSelect.value;
  var inputContainer = document.getElementById("inputContainer");
  inputContainer.innerHTML = "";
  if (selectedOption === "NetworthProjector") {
    createInputField("Current Age:", "currentAge", "22", 22);
    createInputField("Initial Net worth:", "initNw", "10000", 10000);
    createInputField("Yearly income:", "yearlyIncome", "49000", 49000);
    createInputField("Yearly increase in Salary (%):", "yearlyIncrease", "2.0%", 2.0);
    createInputField("Portfolio yearly return (%):", "portfolioReturn", "10.0%", 10.0);
    createInputField("Portfolio fees (%):", "portfolioFees", "2.0%", 2.0);
    createInputField("Percentage Investing (%):", "percentInvest", "15.0%", 15.0);
    createInputField("Age of Retirement:", "ageRetirement", "62", 62);
  } else if (selectedOption === "MortgageCalculator") {
    createInputField("Price:", "price", "", 425000);
    createInputField("Number of months:", "numMonths", "", 360);
    createInputField("Interest rate (%):", "interestRate", "", 4.00);
    createInputField("Housing market inflation (%):", "housingInflation", "", 3.0);
    createInputField("Rent per month to compare:", "rentToCompare", "", 1800.0);
    createInputField("Extra initial expenses (makelaar, overbidding, etc.):", "initExpenses", "", 8000);
    createInputField("Rent annual increase (%):", "rentIncrease", "", 8.000);
  } else if (selectedOption === "PortfolioManager") {
  }
}

function createInputField(labelText, inputId, placeHolderValue, defaultVal) {
  var inputDiv = document.createElement("div");
  var label = document.createElement("label");
  label.setAttribute("for", inputId);
  label.textContent = labelText;
  inputDiv.append(label);
  var inputField = document.createElement("input");
  inputField.setAttribute("type", "number");
  inputField.setAttribute("id", inputId);
  inputField.setAttribute("placeholder", placeHolderValue);
  inputField.value = defaultVal;
  inputDiv.appendChild(inputField);
  var inputContainer = document.getElementById("inputContainer");
  inputContainer.appendChild(inputDiv);
}

function submit() {
  var inputContainer = document.getElementById("inputContainer");
  var inputFields = inputContainer.getElementsByTagName("input");
  var userInput = [];

  for (var i = 0; i < inputFields.length; i++) {
    userInput.push(inputFields[i].value);
  }

  var displayText = document.getElementById("displayText");
  displayText.textContent = "User Input: " + userInput.join(", ");
}