# ClubchatGames
Want to play Clubhouse Games with your Twitch Chat? Here's a solution!

---

Compilation:

- Was built with QMake 3.1, Qt 5.15.0 on Manjaro (Arch-based) Linux.

Dependencies:

- `qt5-base`,  `qt5-serialport`, `libcommuni-3.5.0`

---

![](https://i.imgur.com/7kJdsYt.png) 

Manual Control menu:

![](https://i.imgur.com/YXXsK5g.png) 

Votes Display:

![](https://i.imgur.com/CByo5dw.png) 

Usage:

If using the Wired Pro Controller Interface

 - Connect the Controller to the Switch via the Dock
    - (or a USB-C to A adapter if in handheld.)
 - Connect the Serial Adapter to your PC.
 - Make sure your Switch Firmware is Version 3.0.0 or newer
 -  “Pro Controller Wired Communication” turned on (tutorial below)

1. Open Switch’s settings menu

	* ![Menu](https://i.imgur.com/mcRSoxQ.png  "Menu")

2. Scroll down the left menu to “Controllers and Sensors”
3. Turn ON “Pro Controller Wired Communication”

	* ![Menu](https://i.imgur.com/7wKqBeJ.png  "Menu")
	
 - Continued in shared instructions...

 
If using the Wireless (Joy-Con/Pro-Con) Interface

 - (If you don't want the adapter to be player one, either disconnect the board from power, or hold down the RESET key until you are ready. After either one, you will need to reconnect it to the app if you already connected.)
 - On the Switch's Home Menu, press the Controller Icon on the bottom.
 - Open Change Grip/Order
 - Connect your controllers!
 - If using as a Joy-Con, and it happens to not go into sideways mode automatically, use Manual Control and press the `L+R` button.
 
 
 Shared Instructions
 
 - Open the app, find your adapter's Serial port. (You could plug it in while the app is open, press refresh, and use the newest one)
 
 - Select it, and press Connect.
 
 - You should be able to make sure the PC and Interfaces are talking if it says "Connected!" 
 
 - You can confirm Switch communication via the Manual Control box.
 
 - Type in your Twitch.tv username in the appropriate field, and press Connect.
 
 - The votes display will show commands that your viewers request, and can have the colors changed on the fly for chroma keying.
 
