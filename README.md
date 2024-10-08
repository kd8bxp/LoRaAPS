# Archived Aug 20, 2024

This was an attempt to make something that was a "modern" packet/messaging system. It wasn't the first, as said below it was inspired by Travis Goodspeeds LoRaHam project.  But over the last 4 years better more adapted system have become avaiable, and I haven't worked on this.  I am archiving and keeping for historical reasons, but there other systems that are easier and better at this point.

# LoRaAPS (LoRa Amateur Packet Service)

This project was inspired by Travis Goodspeeds LoRaHam project, and Elvis Pfutzenreuter's LoRaMaDor project.  
This project uses LoRa harware, TTGO LoRa32-Oled, LoRa32u4, and TTGO T-Beam v1. It should be easy to adapt for other boards.  
Full project details can be found here http://kd8bxp.blogspot.com/2020/09/lora-radio-for-amateur-radio-operator.html for more information.  

## Disclaimer

Use this at your own risk, while I believe it is working the way I think it should, testing has been limited. It's hard to test some of the features on the work bench. A wide test is needed.  

There is no guarantee a message will be delivered to the intended person, this should not be used to pass "emergancy" traffic. Please use reliable well tested radios for any and all emergancy traffic.  

Traffic sent over the internet should be considered to be sent in the clear, in otherwords do not send private or personal information, or otherwise sentive information. The whole network should be considered to be in the clear, although there will be a limited number of people to hear OTA traffic (at this point.)  

This project should be considered to be experimental, expect firmware to be updated. Keep this in mind if you want to deloy remote stations - you may need away to update the firmware easy.  

## Version

September 2020 - test of software version 0 to version 0.0.5 (current)  
September 14, 2020 - corrected version numbers inside sketches, changed "gateway", "pager" sketches to add serial output of message recieved. Gateway just outputs the recieved JSON string as is, Pagers will format the output the same as on the OLED display.  
September 16, 2020 - v0.0.7 removed "F" from transmit, changed checks to check "P" 0 for calsign (this should save a little more space in the json string). Added voltage reading for the T-Beam device  
September 19, 2020 - v0.0.8 added button (control), added display for "P" path for the pagers (T-Beam, LoRa32)  

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## Support Me

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  
https://www.paypal.me/KD8BXP  

## Other Projects

https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  


## Credits

Copyright (c) 2020 LeRoy Miller

## License

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
