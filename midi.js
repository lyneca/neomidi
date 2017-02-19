var jazz = require('jazz-midi');
var midi = new jazz.MIDI();

var SerialPort = require('serialport')
var port = new SerialPort('COM16', {parser: SerialPort.parsers.readline('\n')})
var msgn = 0;
var last_note;

port.on('data', function(data) {
  msgn++;
  console.log(msgn + ': ' + data);
})

port.on('open', function() {
    var name = midi.MidiInOpen(0, function(t, msg) {
        var send;
        if (msg[0] == 144) {
            send = '1' + String.fromCharCode(msg[1]);
        } else if (msg[0] == 128) {
            send = '0' + String.fromCharCode(msg[1]);
        }
        if (send) {
            // if (msg[0] == 128 && msg[1] != last_note) {
                // console.log("Ignoring '" + send + "'");
            // } else {
            console.log("Sending  '" + send + "'")
            last_note = msg[1];
            port.drain(function() {
              port.write(send + '\n', function(err) {
                  if (err) return console.log('Error on write: ', err);
                  port.drain();
              });
            })

            // }
        }
    });
})

port.on('error', function(err) {
    console.log('Error: ', err.message);
})
