# Why This Exist
This was a hoby project, Purpose of this project was to create program that uploads everything in a directory to your local server as is.

## Client is Windows app and its uses
* C++20 filesystem library
* WinHttpClient for networking
* Base64 encoding


## Server is written with NodeJS and has following features
* Generates a key for upload authentication, key has a lifespan of 1 second
* Using SHA256 for keys
* Using simple rotating bits for encrypting & decrypting req and responds between client and server


# How To use

 ```
dropbox-client.exe TestProjectUpload
```

or you can give it a target path

```
dropbox-client.exe TestProjectUpload "C:\Users\kurul\Desktop\Project To Upload"
```


# Example 
![](https://github.com/salimhankurul/dropbox-clone/blob/main/gif1.gif)

### TODO's when have free time
* Add file extension filtering
* Add 2D GUI using Imgui
* Detect file changes and automate upload process
* Maybe create login & registering system



