# PHANTOM-IP-Core-Marketplace

This is the Repository for PHANTOM IP Cores.

It contains several IP Cores that implement common algorithms for signal and image processing.
This IP Cores have been designed to work with the PHANTOM Platform.

The scripts and tools to integrate the IP Cores in a design ready to be used by applications running in a Linux enviroment can be found here:

[PHANTOM Platform - Linux Software Distribution](https://github.com/PHANTOM-Platform/PHANTOM-FPGA-Linux)


&nbsp;

## Available IP Cores

### DWT - Discrete wavelet transform

The discrete wavelet transform (DWT) is an implementation of the wavelet transform using a discrete set of the wavelet scales and translations obeying some defined rules. In other words, this transform decomposes the signal into mutually orthogonal set of wavelets. It has recently become a very popular when it comes to analysis, de-noising and compression of signals and images.


##### IP Core Input
-  1 x Image (128 x 2048) - Float array

##### IP Core Outputs
-  4 x Image (64 x 1024) - Float array


#### IP Core Adapter
The IP Core is provided with an adapter that acts as the IP Core driver to ease integration. The adapter uses the IP Core to process a larger image in blocks.


`int dwt(float *inImage, float *outImage0, float *outImage1, float *outImage2, float *outImage3);`


##### IP Core Adapter Input
-  1 x Image (2048 x 2048) - Float array

##### IP Core Adapter Outputs
-  4 x Image (1024 x 1024) - Float array


For the adapter to know which UIO device to use, the IP Core instance name needs to be defined as is in the Device Tree.  
The instance name can be edited in the IP Core adapter header file.

```
// IP Core Name in the Device Tree  
const char * instanceName = "dwt";
```



Example IP Core definition in the Device Tree

```
dwt@43C00000 {
compatible = "phantom_platform,generic-uio,ui_pdrv";
#address-cells = <1>;
#size-cells = <1>;
reg = <0x43C00000 0x10000>;
};
```


&nbsp;  

## iDWT - Inverse Discrete wavelet transform
The inverse discrete wavelet transform, as the name states, does the inverse procedure of the DWT, taking in the decomposed images and re-combining them.

##### IP Core Input
-  4 x Image (64 x 2048) - Float array

##### IP Core Outputs
-  1 x Image (128 x 1024) - Float array


#### IP Core Adapter
The IP Core is provided with an adapter that acts as the IP Core driver to ease integration. The adapter uses the IP Core to process a larger image in blocks.


`int idwt(float *inImage0, float *inImage1, float *inImage2, float *inImage3, float *outImage);`


##### IP Core Adapter Input
-  4 x Image (1024 x 1024) - Float array

##### IP Core Adapter Outputs
-  1 x Image (2048 x 2048) - Float array


For the adapter to know which UIO device to use, the IP Core instance name needs to be defined as is in the Device Tree.  
The instance name can be edited in the IP Core adapter header file.

```
// IP Core Name in the Device Tree  
const char * instanceName = "idwt";
```



Example IP Core definition in the Device Tree

```
idwt@43C00000 {
compatible = "phantom_platform,generic-uio,ui_pdrv";
#address-cells = <1>;
#size-cells = <1>;
reg = <0x43C00000 0x10000>;
};
```


&nbsp;  

## Reserved Memory

The IP Cores use a pre-defined area of DDR Memory that must be reserved for the IP Core exclusive use.
This following block must be added to the device tree.

```
uio_reserved: uioreserved@38000000 {
 no-map;
 reg = <0x38000000 0x8000000>;
};
```


&nbsp;  

## Testing

To check if everything was well defined in the device tree the following commands can be used  

List UIO device names
```
root@phantomfpga:~# for dir in /sys/class/uio/uio*; do cat $dir/name; done
dwt
idwt
```

Show UIO device info
```
root@phantomfpga:~# cat /sys/class/uio/uio0/name
dwt
```
```
root@phantomfpga:~# cat /sys/class/uio/uio0/maps/map0/addr
0x43c00000
```
```
root@phantomfpga:~# cat /sys/class/uio/uio0/maps/map0/offset
0x0
```
```
root@phantomfpga:~# cat /sys/class/uio/uio0/maps/map0/size
0x00010000
```
