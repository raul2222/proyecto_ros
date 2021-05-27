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



conectado = false
/*
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
}*/

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
  document.getElementById("btn_sn").addEventListener("click", call_move_service)
  document.getElementById("btn_rn").addEventListener("click", call_route_service)

  data = {
      // ros connection
      ros: null,
      rosbridge_address: 'ws://127.0.0.1:9090/',
      connected: false,
  }

  function connect(){
      console.log("Clic en connect")

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
          document.getElementById('divCamera').innerHTML = "" 
      })
  }

  function disconnect(){
      data.ros.close()        
      data.connected = false
      console.log('Clic en botón de desconexión')
  }    

  function call_route_service(){
    data.service_busy = true
    data.service_response = ''
  
  //definimos los datos del servicio
    let service = new ROSLIB.Service({
        ros: data.ros,
        name: '/route_navigation',
        serviceType: 'route_navigation_service_msg/RouteNavigationServiceMessage'
    })
  
  //definimos el parámetro de la llamada
    let request = new ROSLIB.ServiceRequest({
        //point: parseInt(document.getElementById("point").value)
    })
  
    service.callService(request, (result) => {
        data.service_busy = false
        data.service_response = JSON.stringify(result)
    }, (error) => {
        data.service_busy = false
        console.error(error)
    })	
  }
  
  
  //conflicto con move
  
  function call_move_service(){
    data.service_busy = true
    data.service_response = ''
  
  //definimos los datos del servicio
    let service = new ROSLIB.Service({
        ros: data.ros,
        name: '/simple_navigation',
        serviceType: 'simple_navigation_service_msg/SimpleNavigationServiceMessage'
    })
  
  //definimos el parámetro de la llamada
    let request = new ROSLIB.ServiceRequest({
        point: parseInt(document.getElementById("point").value)
    })
  
    service.callService(request, (result) => {
        data.service_busy = false
        data.service_response = JSON.stringify(result)
    }, (error) => {
        data.service_busy = false
        console.error(error)
    })	
  } 
  
});

/*
function disconnect(){
  data.ros.close()
  data.connected = false
  console.log('Clic en botón de desconexión')
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
}*/