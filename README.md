> # T9-Usage-display
> The code for using a T-Mobile T9 (Franklin Wireless R717) As a display with a Raspberry pi 4

> ## Usage
>> 1. Enable adb through ssh
>> 2. Compile code
>> 3. Connect to device with usb cable
>> 4. Profit

> ## Getting Access to the hotspot
> https://snt.sh/2020/09/rooting-the-t-mobile-t9-franklin-wireless-r717/

> ### Enable SSH
>> ```bash
>> curl "http://192.168.0.1/cgi-bin/webpst.service_setting.cgi" \
>>  -H "Content-Type: application/json" \
>>  -H "Origin: http://192.168.0.1" \
>>  -H "Referer: http://192.168.0.1/webpst/usb_mode.html" \
>>  --data '{"command":"save","params":null,"data":{"ssh":"on","tether":"","bridge":""}}' \
>>  --insecure
>> ```
> ### Enable ADB
>> Edit the /data/configs/mobileap_cfg.xml file. Update the UsbMode setting value from 9025 to 902D, save, then reboot the device. Note you also may need to replace the contents data/configs/hsusb_next with 902D as well.
> ### Commands piped through adb shell
>> Display Text 
>>> ```sh     
>>> /usr/bin/guimgr_cli lcd_eng_mode xMessageHere
>>> ```
>
>> reset
>>> ```sh     
>>> /usr/bin/guimgr_cli lcd_eng_mode 0
>>> ```
>
>> Blank the LCD
>>> ```sh     
>>> echo 1 > /sys/class/graphics/fb0/blank
>>> ```
>
>> TV static
>>> ```sh     
>>> cat /dev/urandom > /dev/fb0
>>> ```
> ### Other Notes
>> It seems overall they use nano-x as the GUI manager. If you want to bit bang > raw data to the LCD, /dev/fb0 is your man. You can kill the process that draws to the buffer (nano-X) and it won’t clear it on you anymore, but you’ll lose the backlight control. It’s 128x36px, 1 bit per pixel.
>
>> You can control the backlight yourself, but you’ll need a helper program for it. Cross complies on ubuntu with arm-linux-gnueabi-gcc just fine
>>>    On: ioctl(28, _IOC(0, 0x00, 0xc8, 0x00), 0)\
>>>    Off: ioctl(21, _IOC(0, 0x00, 0xc9, 0x00), 0)
>>>>        Where 28/21 is just an fd to /dev/fb0
>>> ```c
>>> #include
>>> #include
>>> int main() {
>>> return ioctl(open(“/dev/fb0”, O_RDWR), _IOC(0, 0x00, 0xc8, 0x00));
>>> }
>>> ```
