// configuracion parqa firebase, no modificar esto ni el firebase.js si no es necesario
var uiConfig = {
  signInSuccessUrl: 'basic.html',
  signInOptions: [
          firebase.auth.EmailAuthProvider.PROVIDER_ID,
  ],
  // Terms of service url.
  tosUrl: 'index.html'
};

// Inicializacion de firebase
var ui = new firebaseui.auth.AuthUI(firebase.auth());
// lo lanzara cuando este cargado
ui.start('#firebaseui-auth-container', uiConfig);