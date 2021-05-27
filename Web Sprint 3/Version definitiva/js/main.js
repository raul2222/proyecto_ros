var mainApp = {};
//
(function(){
  var mainContainer = document.getElementById("main_container");
  
      //desloguearse
      var logtout =  function(){
          firebase.auth().signOut().then(function(){
              console.log('success');
              window.location.replace("login.html");
          },function(){})
      }
  
  var init = function(){
      firebase.auth().onAuthStateChanged(function(user) {
          if (user) {
            // Usuario esta dentro
            mainContainer.style.display = "";
          } else {
            // el usuario no estaba dentro
            mainContainer.style.display = "none";
            window.location.replace("login.html");
          }
        });
  }
  
  
  
  
  init();
  
  mainApp.logout = logtout;
  })();



//


