
## Welcome to DropGen! A droplet on demand generator for research applications developed in the Harris Lab at Brown University

<p align="center">
  <img src="https://github.com/harrislab-brown/Filament_DropGen/blob/main/DOCUMENTATION/DGD_00.jpeg" width="800">
</p>


DropGen uses a fluid reservoir and a piezoelectric buzzer to precisely and repeatably dispense small droplets on demand. 
This new version of the Harris Lab DropGen is designed to be 3D printed using filament based FDM printers. Harris Lab has developed a custom printed circuit board to make assembly and use of DropGen as easy as possible, including adding the ability to control the height of DropGen with a commonly available stepper motor driven linear actuator. 

### WIKI:
Complete build documentation and user guide for DropGen can be found under the [Wiki](https://github.com/harrislab-brown/Filament_DropGen/wiki) tab of this repository.

### CAD: 
The CAD directory contains design files for the droplet generator 3D printed parts and the custom PCB. 
Cad files are available as a .f3z Fusion360 archive file, as well as program agnostic .step files for modification in parametric cad software. All parts that require 3D printing are also available in .stl format which can be used with all modern 3D printing slicers. 

The DropGenPCB directory contains project files for the printed circuit board. The PCB was designed using KiCad 8. Production files ready to send to JLCPCB are available here as well (see wiki for details). 

### CODE: 
DropGen is controlled by a web based GUI running on an ESP32-S3 development board. The wiki contains instructions on how to set up the environment required to flash the ESP32 firmware. 

### Documentation: 
This directory contains image and video assets used in the wiki. 

Happy building! 
