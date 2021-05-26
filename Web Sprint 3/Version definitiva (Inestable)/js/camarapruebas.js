var mainApp = {};
//

document.addEventListener('DOMContentLoaded', event => {
    
});


function setCamera(){
  console.log("setting the camera")
  let viewer1 = new MJPEGCANVAS.Viewer({
      divID: "divCamera", //elemento del html donde mostraremos la cámara
      host: "127.0.0.1:8080", //dirección del servidor de vídeo
      width: 320, //no pongas un tamaño mucho mayor porque puede dar error
      height: 240,
      topic: "/turtlebot3/camera/image_raw",
      ssl: false,
  })
}
document.getElementById("btn_con").addEventListener("click", connect)
document.getElementById("btn_dis").addEventListener("click", disconnect)
conectado = false
data = {
  // ros connection
  ros: null,
  rosbridge_address: 'ws://127.0.0.1:9090/',
  connected: false
}
function connect(){
  data.ros = new ROSLIB.Ros({
      url: data.rosbridge_address
  })

  // Define callbacks
  data.ros.on("connection", () => {
      data.connected = true
      console.log("Conexion con ROSBridge correcta") 
  })
  data.ros.on("error", (error) => {
      console.log("Se ha producido algun error mientras se intentaba realizar la conexion")
      console.log(error)
  })
  data.ros.on("close", () => {
      data.connected = false
      document.getElementById("divCamera").innerHTML = ""
      console.log("Conexion con ROSBridge cerrada")             
  })
}

function conexionCompleta(data){
  
  let topic = new ROSLIB.Topic({
    ros: data.ros,
    name: '/odom',
    messageType: 'nav_msgs/Odometry'
  })
  topic.subscribe((message) => {
    data.position = message.pose.pose.position
    document.getElementById("pos_x").innerHTML = data.position.x.toFixed(2)
    document.getElementById("pos_y").innerHTML = data.position.y.toFixed(2)
  }) 

  if(conectado == false){
    conectado = true
     data.mapViewer = new ROS2D.Viewer({
        divID: 'map', //id del objeto html
        width: 420, 
        height: 360
  })
  data.mapGridClient = new ROS2D.OccupancyGridClient({
        ros : data.ros,
        rootObject : data.mapViewer.scene,
        continuous: true //este atributo permite mantener el mapa actualizado cuando se cambie por el algoritmo de mapeado del robot
  })
  data.mapGridClient.on('change', () => {
        data.mapViewer.scaleToDimensions(data.mapGridClient.currentGrid.width, data.mapGridClient.currentGrid.height)
        data.mapViewer.shift(data.mapGridClient.currentGrid.pose.position.x, data.mapGridClient.currentGrid.pose.position.y)
})
  }
 

};

document.addEventListener('DOMContentLoaded', event => {
  console.log("entro en la pagina")
  
  document.getElementById("btn_con").addEventListener("click", connect)
  document.getElementById("btn_dis").addEventListener("click", disconnect)

  data = {
      // ros connection
      ros: null,
      rosbridge_address: 'ws://127.0.0.1:9090/',
      connected: false,
  }

  function connect(){
      console.log("Intentando connectar")

      data.ros = new ROSLIB.Ros({
              url: data.rosbridge_address
      })

      // Define callbacks
      data.ros.on("connection", () => {
          data.connected = true
          console.log("Conexion con ROSBridge correcta")
          setCamera()
          conexionCompleta(data)
          
      })
      data.ros.on("error", (error) => {
          console.log("Se ha producido algun error mientras se intentaba realizar la conexion")
         
          console.log(error)
          console.log("Reintentando....")
          connect()
      })
      data.ros.on("close", () => {
          data.connected = false
          conectado = false
          data.ros.on('close', () => {
            data.connected = false
            console.log('Connection to ROSBridge was closed!')
           
        })
          console.log("Conexion con ROSBridge cerrada")            
          document.getElementById('map').innerHTML = "" 
      })
  }

  function disconnect(){
      data.ros.close()        
      data.connected = false
      console.log('Clic en botón de desconexión')
  }    
  
});

function disconnect(){
  data.ros.close()
  data.connected = false
  console.log('Clic en botón de desconexión')
}  
(function(){
var mainContainer = document.getElementById("main_container");

    //desloguearse
    var logtout =  function(){
        firebase.auth().signOut().then(function(){
            console.log('success');
            window.location.replace("login.html");
        },function(){})
    }


document.getElementById("btn_move").addEventListener("click", move)

function move() {
  let topic = new ROSLIB.Topic({
      ros: data.ros,
      name: '/cmd_vel',
      messageType: 'geometry_msgs/Twist'
  })
  let message = new ROSLIB.Message({
      linear: {x: 1, y: 0, z: 0, },
      angular: {x: 0, y: 0, z: 0.5, },
  })
  topic.publish(message)
}

var init = function(){
    firebase.auth().onAuthStateChanged(function(user) {
        if (user) {
          // Usuario esta dentro
          mainContainer.style.display = "";
          connect()
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