import React, { Component } from "react";
import logo from "./logo.svg";
import "./App.css";
import config from "./config";

import { Avatar, Button, Grid } from "@material-ui/core";
import TextField from "@material-ui/core/TextField";
import Container from "@material-ui/core/Container";

import openImg from "./resource/open.png";
import closeImg from "./resource/close.png";

/* eslint-disable */

const microgear = Microgear.create({
  key: config.appKey,
  secret: config.appSecret,
  alias: "web2",
});

class App extends Component {
  constructor(props) {
    super(props);
    this.state = { time: null, msg: "OFF",value:0 };
    // this.onSwitch = this.onSwitch.bind(this);
    this.onMessage = this.onMessage.bind(this);
    // microgear.on("message", this.onMessage);
    // microgear.on("connected", this.onconnected);

    // microgear.connect(config.appId);
    // microgear.subscribe(config.topic);
  }

  componentDidMount() {
    microgear.on('message', this.onMessage);
    microgear.on('connected', this.onconnected);

    microgear.connect(config.appId);
    microgear.subscribe(config.topic);
  }

  onconnected() {
    microgear.setAlias("nodeMCU");
    // console.log("connected");
  }

  onMessage(topic, msg2) {
    console.log(msg2);
    const nowTime = new Date().getUTCHours() + ":" + new Date().getUTCMinutes();
    
    if(msg2==-1){
      this.setState({ time: nowTime, msg: "OFF" });
    }
    else{
      this.setState({ time: nowTime, msg: "ON" ,value:msg2});
    }
  }

  onSwitch = (event) => {
    const topic = config.topic;
    // this.setState({ waiting: true });
    event.preventDefault();
    // microgear.publish(topic, event.target.text.value );
    // console.log(event.target.text.value)
    // console.log(event.target.text.value)
    microgear.chat("nodeMCU", event.target.text.value);
  };

  render() {

    let pict=null;
    if(this.state.msg){
      pict=(<img src={this.state.msg==='ON'?openImg:closeImg}   ></img>)
    }
    return (
      <div className="App">
        <Container>
          <p>time {"[" + this.state.time + "] : " + this.state.msg +" : "+this.state.value}</p>
          {/* <Grid
            container
            style={{
              backgroundColor: "#cfe8fc",
              width: "auto",
              margin: "0 auto",
            }}
          > */}
          {pict}
          <form onSubmit={this.onSwitch}>
            <TextField
              id="text"
              label="Your Text"
              variant="outlined"
              autoComplete="off"
            />
            <br />
            <br />
            <Button type="submit" variant="contained" color="primary">
              Click Me
            </Button>
          </form>
          {/* </Grid> */}
        </Container>
      </div>
    );
  }
}

export default App;
