const SerialPort = require("serialport");

const port = 'COM3';

const sp = new SerialPort(port, {
  baudrate: 115200,
  parser: SerialPort.parsers.readline("\n")
});

const onPortOpen= () =>
    console.log(`Port ${port} opened.`);
 
const onData = (d) =>
    console.log(d);
 
const onClose = () =>
    console.log(`Port ${port} opened.`);

const onError = (e) => 
	console.error(e);

sp.on('open', onPortOpen);
sp.on('data', onData);
sp.on('close', onClose);
sp.on('error', onError);


setTimeout(()  => {
	sp.write('1031', (err) => {
		if(err)
			console.err('Error on write', err.message);
	})
}, 3000);
