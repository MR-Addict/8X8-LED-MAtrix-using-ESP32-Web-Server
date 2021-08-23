const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>ESP32 Web Server</title>
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
        font-size: 1.3rem;
        font-weight: bold;
      }
      .box {
        width: 25rem;
        margin: auto;
        background: #50d890;
        padding: 1rem;
        box-sizing: border-box;
        color: #fff;
        border-radius: 1rem;
        box-shadow: 5px 5px 5px black;
      }
      button {
        width: 8rem;
        height: 3rem;
        margin: 1rem 1rem;
        border-radius: 1rem;
        font-size: 1.3rem;
        background: #f4f9f9;
      }
      button:hover {
        cursor: pointer;
      }
      #slider {
        -webkit-appearance: none;
        appearance: none;
        width: 17.5rem;
        height: 0.5rem;
        background: #3db2ff;
        outline: none;
        opacity: 1;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
        border-radius: 1rem;
      }
      #slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 1.2rem;
        height: 1.2rem;
        background: #ae00fb;
        cursor: pointer;
        border-radius: 50%;
      }
      #slider::-moz-range-thumb {
        width: 2rem;
        height: 2rem;
        background: #ae00fb;
        cursor: pointer;
      }
      .switch {
        position: relative;
        display: inline-block;
        width: 5rem;
        height: 2.5rem;
      }
      .switch input {
        display: none;
      }
      .slider {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        border-radius: 3rem;
      }
      .slider:before {
        position: absolute;
        content: "";
        height: 2rem;
        width: 2.5rem;
        left: 0.4rem;
        bottom: 0.25rem;
        background-color: white;
        -webkit-transition: 0.4s;
        transition: 0.4s;
        border-radius: 3rem;
      }
      input:checked + .slider {
        background-color: #2196f3;
      }
      input:checked + .slider:before {
        -webkit-transform: translateX(1.8rem);
        -ms-transform: translateX(1.8rem);
        transform: translateX(1.8rem);
      }
    </style>
  </head>
  <body>
    <div class="box">
      <h1>ESP32 Web Server</h1>
      <label class="switch">
        <input onchange="getSwitch(this)" type="checkbox" id="output" />
        <span class="slider"></span>
      </label>
      <p>
        <button id="Button1" onclick="getButton(0)">Stars</button>
        <button id="Button2" onclick="getButton(1)">Cross</button><br />
        <button id="Button3" onclick="getButton(2)">Chase</button>
        <button id="Button4" onclick="getButton(3)">Pour</button><br />
        <button id="Button5" onclick="getButton(4)">Rotate</button>
        <button id="Button6" onclick="getButton(5)">Rectangle</button><br />
        <button id="Button7" onclick="getButton(6)">Text</button>
        <button id="Button8" onclick="getButton(7)">IP</button><br />
      </p>
      <p>Brightness:<spn id="sliderValue">127</spn></p>
      <p>
        <input
          type="range"
          min="0"
          max="255"
          value="25"
          id="slider"
          onchange="getSlider(this)"
        />
      </p>
    </div>
  </body>
  <script>
    var Buttons = [
      "Button1",
      "Button2",
      "Button3",
      "Button4",
      "Button5",
      "Button6",
      "Button7",
      "Button8",
    ];
    var websocket = new WebSocket("ws://" + location.hostname + ":81/");
    websocket.onopen = function (event) {
      onOpen(event);
    };
    websocket.onclose = function (event) {
      onClose(event);
    };
    websocket.onmessage = function (event) {
      onMessage(event);
    };
    websocket.onerror = function (event) {
      onError(event);
    };
    function onOpen(event) {
      console.log("Server Connected!");
      alert("Server Connected!");
    }
    function onClose(event) {
      console.log("Server Disconnected!");
      alert("Server Disconnected!");
    }
    function onMessage(event) {
      var Switch_Status;
      var sliderValue;
      var Mode;
      var JSONContent = JSON.parse(event.data);
      if (JSONContent.hasOwnProperty("LED")) {
        if (JSONContent.LED == "ON") Switch_Status = true;
        else Switch_Status = false;
        document.getElementById("output").checked = Switch_Status;
      }
      if (JSONContent.hasOwnProperty("sliderValue")) {
        document.getElementById("sliderValue").innerHTML =
          JSONContent.sliderValue;
        document.getElementById("slider").value = JSONContent.sliderValue;
      }
      if (JSONContent.hasOwnProperty("Mode")) {
        for (var i = 0; i < Buttons.length; i++) {
          if (i == JSONContent.Mode)
            document.getElementById(Buttons[i]).style.backgroundColor =
              "#28FFBF";
          else
            document.getElementById(Buttons[i]).style.backgroundColor =
              "#F4F9F9";
        }
      }
    }
    function onError(event) {
      console.log("Error:" + event.data);
      alert("Error Occured!");
    }
    function getSwitch(element) {
      var command;
      if (element.checked) command = '{"LED":"ON"}';
      else command = '{"LED":"OFF"}';
      websocket.send(command);
    }
    function getSlider(element) {
      var sliderValue = document.getElementById("slider").value;
      websocket.send('{"sliderValue":' + sliderValue + "}");
    }
    function getButton(element) {
      websocket.send('{"Mode":' + element + "}");
    }
  </script>
</html>
)rawliteral";
