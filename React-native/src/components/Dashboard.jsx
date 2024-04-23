import React, { useState, useEffect, useRef } from "react";
import { StyleSheet, View, SafeAreaView, Switch, Image } from "react-native";
import { Text } from "@rneui/themed";
import AsyncStorage from "@react-native-async-storage/async-storage";
import init from "react_native_mqtt";

import FontAwesome5 from 'react-native-vector-icons/FontAwesome5'
import FontAwesome from 'react-native-vector-icons/FontAwesome'

let message = "";
let topicSub = "64028780/data";
let topicPub = "64028780/Msg";

init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});

const Dashboard = () => {
  const [humidity, setHumidity] = useState(null);
  const [temperature, setTemperature] = useState(null);
  const [day, setDay] = useState(null);
  const [hours, setHours] = useState(null);
  const [minutes, setMinutes] = useState(null);
  const [ledState, setLedState] = useState(false);
  const [relayState, setRelayState] = useState(false);

  const clientRef = useRef(null);

  useEffect(() => {
    const client = new Paho.MQTT.Client(
      "broker.emqx.io",
      8083,
      "APL_C" + parseInt(Math.random() * 100000)
    );
    clientRef.current = client;

    const onConnect = () => {
      console.log("onConnect");
      client.subscribe(topicSub);
    };

    const onConnectionLost = (responseObject) => {
      if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
      }
    };

    const onMessageArrived = (message) => {
      try {
        const data = JSON.parse(message.payloadString);
        const newHumidity = data.humidity.toFixed(1);
        const newTemperature = data.temperature.toFixed(1);
        const daysUntilAlert = data.daysUntilAlert;
        const hoursUntilAlert = data.hoursUntilAlert;
        const minutesUntilAlert = data.minutesUntilAlert;

        setHumidity(newHumidity);
        setTemperature(newTemperature);
        setDay(daysUntilAlert);
        setHours(hoursUntilAlert);
        setMinutes(minutesUntilAlert);
      } catch (error) {
        console.error("Error parsing message:", error);
      }
    };

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    client.connect({ onSuccess: onConnect });

    return () => {
      client.disconnect();
    };
  }, []);

  const toggleLED = (value) => {
    const ledState = value;
    const messagePayload = JSON.stringify({ LED: ledState });
    const message = new Paho.MQTT.Message(messagePayload);
    message.destinationName = topicPub;
    clientRef.current.send(message);
    setLedState(ledState);
  };

  const toggleRelay = (value) => {
    const relayState = value;
    const messagePayload = JSON.stringify({ Relay1: relayState });
    const message = new Paho.MQTT.Message(messagePayload);
    message.destinationName = topicPub;
    clientRef.current.send(message);
    setRelayState(relayState);
  };

  return (
    <SafeAreaView>
      <View style={styles.tem}>
        <View style={styles.humidity}>
        <FontAwesome5 name="cloud-rain" color={'#00bbf0'} size={46}></FontAwesome5>
          <Text h2>{humidity}</Text>
          <Text h3>humidity</Text>
        </View>
        <View style={styles.temperature}>
        <FontAwesome name="thermometer-half" color={'#ff9a3c'} size={46}></FontAwesome>
          <Text h2>{temperature}</Text>
          <Text h3>temperature</Text>
        </View>
      </View>

      <View style={styles.time}>
        <View style={{width: 150}}>
        <Text h4>ได้ภายใน</Text>
        <Text h4> {day ? day + " วัน " : ""}</Text>
        <Text h4> {hours ? hours + " ชั่วโมง " : ""}</Text>
        <Text h4> {minutes ? minutes + " นาที" : ""}</Text>
    
        </View>
       
       <View style={{width:200,height:200}}>
            <Image source={require('../img/Logo_-01-1.jpg')}  style={styles.tinyLogo}></Image>
       </View>
      </View>

       
      <View style={styles.swt}>
        <Switch
          value={ledState}
          onValueChange={toggleLED}
          style={{ transform: [{ scaleX: 1.5 }, { scaleY: 1.5 }] }}
        />
        <Text h4> LED</Text>
        <Switch
          value={relayState}
          onValueChange={toggleRelay}
          style={{ transform: [{ scaleX: 1.5 }, { scaleY: 1.5 }] }}
        />
        <Text h4> Relay</Text>
      </View>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  tem: {
    flexDirection: "row",
    margin: 10,
    backgroundColor: "#fefefe",
    height: 300,
    borderRadius: 30,
    justifyContent: "space-evenly",
  },
  humidity: {
    width: "40%",
    alignItems: "center",
    justifyContent: "center",
  },
  temperature: {
    width: "40%",
    alignItems: "center",
    justifyContent: "center",
  },
  time: {
    flexDirection: 'row',
    margin: 10,
    backgroundColor: "#fefefe",
    borderRadius: 30,
    padding: 20,
  },
  swt: {
    flexDirection: "row",
    width: "100%",
    height: 100,
    alignItems: "center",
    padding: 20,
    justifyContent: "space-around",
  },
  tinyLogo:{
    width:'100%',
    height:'100%',
    objectFit:'cover',
    borderRadius:20

  }
});

export default Dashboard;
