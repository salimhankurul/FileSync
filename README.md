# Why This Exist
This was a hoby project, Purpose of this project was to create program that upload folder and its subdirectory to your local server.
It clears clutter by combining 2D box that colliding to each other and makes them 1 2D box

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

or   or you can give it a target path

```
dropbox-client.exe TestProjectUpload "C:\Users\kurul\Desktop\Project To Upload"
```


# Example 
![](https://github.com/salimhankurul/dropbox-clone/blob/main/gif1.gif)


