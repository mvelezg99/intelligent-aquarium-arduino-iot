(function () {

    // Set the configuration for your app
    // TODO: Replace with your project's config object
    const config = {
        apiKey: "tJUm9XpFGig8lPEkxVIEzqRtV0ZnunTJpzCbKD7b",
        authDomain: "intelligent-aquarium-iot.firebaseapp.com",
        databaseURL: "https://intelligent-aquarium-iot.firebaseio.com/",
        storageBucket: "intelligent-aquarium-iot.appspot.com"
    }
    firebase.initializeApp(config)

    // Get a reference to the database service
    const textTemperature = document.getElementById('text-temperature')
    const textNivel = document.getElementById('text-nivel')
    const textpH = document.getElementById('text-ph')

    const dbRefObjectTemperatura = firebase.database().ref().child('IntelligentAquarium').child('temperatura')
    const dbRefObjectNivel = firebase.database().ref().child('IntelligentAquarium').child('distancia')
    const dbRefObjectpH = firebase.database().ref().child('IntelligentAquarium').child('pH')

    let temperature = {}
    let nivel = {}
    let pH = {}

    dbRefObjectTemperatura.on('value', snap => {
        console.log(snap.val())
        temperature = JSON.stringify(snap.val())
        textTemperature.innerHTML = `<img src="res/temperature-logo.png" width="50" height="50"> Temperatura: ${temperature} °C`

    })

    dbRefObjectNivel.on('value', snap => {
        nivel = JSON.stringify(snap.val())
        textNivel.innerHTML = `<img src="res/waterlevel-logo.png" width="50" height="50"> Nivel: ${nivel} cm`
    })

    dbRefObjectpH.on('value', snap => {
        pH = JSON.stringify(snap.val())
        textpH.innerHTML = `<img src="res/ph-logo.png" width="50" height="50"> pH: ${pH}`
    })

}())