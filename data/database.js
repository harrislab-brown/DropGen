var Socket;
var dropsLeft = 0;
var timerID = null;
var intervalID = null;
const defaultValues = ['1000', '500', '100', '1', '15'];
const elementIds = ['plsw', 'dpdel', 'dpnum', 'trvl', 'cmdel', 'resPos'];

window.onload = function(event){
  Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  Socket.onopen = function(event) {};
  Socket.onmessage = function(event) {receive_data(event)};
  Socket.onerror = function(error) {};
}

function updateDrops(){
  var dropNum = document.getElementById('dpnum').innerHTML;
  dropNum = parseInt(dropNum.innerHTML, 10) - 1;
  dropsLeft--;
}

function calculateTime(isGo){
  if(isGo){
    send_data(true);
    changeBtnState('stop', 'go', '#E34234', ' 0 9px #A42A04');
    setTimeout(function() {changeBtnState('go', 'stop', '#B4C424', ' 0 9px #90913c');}, 3000);
    sendBtnData("/go");
  } else {
    if(timerID != null){clearTimeout(timerID);}
    sendBtnData("/stop");
    changeBtnState('go', 'stop', '#B4C424', ' 0 9px #90913c');
    setTimeout(function() {changeBtnState('stop', 'go', '#E34234', ' 0 9px #A42A04');}, 3000);
  }
}

function changeBtnState(onButton, offButton, color, shadow) {
  var btn1 = document.getElementById(onButton);
  var btn2 = document.getElementById(offButton);
  btn1.style.backgroundColor = color;
  btn1.style.boxShadow = shadow;
  btn1.style.cursor = 'default';
  btn1.disabled = false;
  // btn2.style.backgroundColor = '#ADAEB3';
  // btn2.style.boxShadow = ' 0 9px #8A8B8F';
  // btn2.style.cursor = 'not-allowed';
  // btn2.disabled = true;
}

function sendBtnData(btn){
  var xhttp = new XMLHttpRequest();
  xhttp.open('PUT', btn, true);
  xhttp.send();
}


function send_data(isGo = false) {
  let result = "";
    for (let i = 1 ; i <= defaultValues.length ; i++) { 
      let value = document.getElementById(`textbox${i}`).value;
      result += "p" + (/\d/.test(value) ? value : defaultValues[i - 1]);
    }

    let prevResMotPos = parseFloat(document.getElementById("resPos").innerHTML.replace(',', '')).toFixed(1); 
    prevResMotPos = (/\d/.test(prevResMotPos) ? prevResMotPos : "0.00");

    if(isGo){
      let resMotPos = parseFloat(document.getElementById("textbox6").value).toFixed(1);
      result += "p" + (/\d/.test(resMotPos) ? resMotPos : prevResMotPos);
    } else {
      result += "p" + prevResMotPos;
    }
    Socket.send(result);
  }

function receive_data(event) {
  
  const myArray = event.data.split(' ');
  var wrapper = document.querySelector(".wrapper");
  var checkbox = document.getElementById("camera");
  var toggleCam = document.getElementById('toggle-cam');
  var toggleGen = document.getElementById('toggle-gen');
  if (myArray.length >= 2) {
    
    // Update Generator Toggle Data
    const isGenOn = myArray[0] === '1';
    toggleGen.innerHTML = isGenOn ? 'On' : 'Off';
    toggleGen.style.backgroundColor = isGenOn ? '#B4C424' : '#E34234';
    toggleGen.style.boxShadow = isGenOn ? ' 0 9px #90913c' : ' 0 9px #A42A04';
    wrapper.classList.toggle("start", isGenOn);

    // Update Camera Toggle Data
    const isCamOn = myArray[1] === '1';
    toggleCam.innerHTML = isCamOn ? 'On' : 'Off';
    toggleCam.style.backgroundColor = isCamOn ? '#B4C424' : '#E34234';
    toggleCam.style.boxShadow = isCamOn ? ' 0 9px #90913c' : ' 0 9px #A42A04';
    checkbox.checked = isCamOn;

    // Update Other Elements
    elementIds.forEach((id, index) => {
      document.getElementById(id).innerHTML = myArray[index + 2];
    });
  }
  const dropDelay = document.getElementById('dpdel').innerHTML;
  if(toggleGen.innerHTML === 'On') {
    if(intervalID === null){
      dropsLeft = myArray[3];
      setInterval(updateDrops, parseInt((dropDelay) + 10), dropNum);
    }
  } else if (dropsLeft <= 0 || toggleGen.innerHTML === 'Off'){
    if(intervalID !== null){
      clearInterval(intervalID);
      intervalID = null;
    }
  }
  
};
