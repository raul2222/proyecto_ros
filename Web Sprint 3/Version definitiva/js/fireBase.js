var fireBase = fireBase || firebase;
var hasInit = false;
var config = {
    apiKey: "AIzaSyBaXLQra8pV0-HmPO4QpCfQHaMjCZlWOIk",
    authDomain: "robotdomotico.firebaseapp.com",
    databaseURL: "https://robotdomotico.firebaseio.com",
    projectId: "robotdomotico",
    storageBucket: "robotdomotico.appspot.com",
    messagingSenderId: "106011592202",
    appId: "1:106011592202:web:91aabad2cb65fdc35201f3"
  };
if(!hasInit){
    firebase.initializeApp(config);
    hasInit = true;
}


