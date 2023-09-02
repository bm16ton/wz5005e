A esp8266 firmware to control the wz5005 (small no builtin psu, nor wifi support model, other WZ models may work to, untested) 

What works, setting voltage and amps, turning output on/off, tho seperate button for each cuz of uhm issues

Wiring, I dont swap serial and connect serial0 receive to PSU, I also enable serial1 and connect its tx to PSU, with this config my two serial streams dont get mingled (PSU/DEBUG) but now I cannot program it without cutting power to PSU first. Strange that wasnt an issue when both rx and tx where connected to serial0. 

Original firmware was for dps500X series with totally different protocol, can be found here.  https://github.com/zsellera/dps-wifi

I sniffed the stock wz5005 windows app from manufacturer for protocol and spent a couple minutes with programmers calculator for crc. But a gentleman has posted the protocol. Manufacturer has as well and you can find it on-line but its always for the larger model with more features. I cant remember if/what may have been different between the two (except mini not having things like built-in wifi support etc) but behaviors are different. For one the larger model once connected via uart/wifi etc you can no longer control the device via its buttons, the small one you can....tho i can see some possible problems with that. the mans documented protocol is here.  https://github.com/kordian-kowalski/wz5005-control.git   ps: last i knew he didnt have the crc which is simple, add the all hex numbers in message up then divide by 100, instead of dealing with floats etc just addem up and take the last 2 digits.

UPDATE; When things get messy on the serial stream, I can (currently by accident) make it so the buttons no longer work when in serial control mode...Dunno how. I also have seen a couple "undocumented" (if thats what were calling what we have) message types, I believe one is AA 1 18 but I m,ay be mis-remembering  


Inside the folder bens_scripts is the inner workings of an obviously broken mind. 

TODO set/get CC and CV, reading TEMP, and Voltage in reading. The only others I would be interested in are the error/alerts 
