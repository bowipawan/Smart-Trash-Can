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
  alias: "web",
});

class App extends Component {
  constructor(props) {
    super(props);
    this.state = { time: null, msg: null };
    // this.onSwitch = this.onSwitch.bind(this);
    this.onMessage = this.onMessage.bind(this);
    microgear.on("message", this.onMessage);
    // microgear.on("connected", this.onconnected);

    // microgear.connect(config.appId);
    // microgear.subscribe(config.topic);
  }

  componentDidMount() {
    microgear.connect(config.appId);
    // microgear.subscribe(config.topic);
  }

  onconnected() {
    microgear.setAlias("nodeMCU2");
    console.log("connected");
  }

  onMessage(topic, msg2) {
    // console.log(msg2);
    const nowTime = new Date().getUTCHours() + ":" + new Date().getUTCMinutes();
    this.setState({ time: nowTime, msg: msg2 });
  }

  onSwitch = (event) => {
    // const topic = config.topic;
    // this.setState({ waiting: true });
    event.preventDefault();
    // microgear.publish(topic, 'ON' );
    // console.log(event.target.text.value)
    microgear.chat("nodeMCU", event.target.text.value);
  };

  render() {
    return (
      <div className="App">
        <Container>
          <p>time {"[" + this.state.time + "] : " + this.state.msg}</p>
          {/* <Grid
            container
            style={{
              backgroundColor: "#cfe8fc",
              width: "auto",
              margin: "0 auto",
            }}
          > */}
          <Avatar src={closeImg} sizes="500" style={{ width: "50" }}></Avatar>
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
