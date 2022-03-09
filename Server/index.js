/* By Han Kurul*/
const express = require("express");
const http = require("http");
const corse = require("cors");

const PORT = process.env.PORT || 5000;
const router = express.Router();

const app = express();
const server = http.createServer(app);
const path = require('path');
const fs = require('fs');
const util = require('util');
const crypto = require('crypto');
const fsextra = require('fs-extra');

app.use(router);
app.use(corse());

const {
    rorInt8,
    rolInt8,
    rorInt16,
    rolInt16,
    rorInt32,
    rolInt32
} = require("./LocalFiles/bitrotator.js");


let AuthKeys = [];

const BadResponse = "404";
const AuthLifeTime = 3000;
const UserAgent = "blackmamba";
const version = "1.2.0";

function sleep(time) {
    return new Promise((resolve) => setTimeout(resolve, time));
}

async function CheckAuthKey(authkey) {

    // Validate Auth Key
    if (!authkey)
        console.log("AR::AutKey Failed: No Auth key");


    if (!AuthKeys.includes(authkey)) {
        console.log("AR::ValidAutKey Failed: " + authkey.substr(0, 16) + "...");
        return false;
    } else
        //console.log("AR::ValidAutKey Success: " + authkey.substr(0,16) + "...");


        return true
}

const aes256gcm = (key) => {
    const ALGO = 'aes-256-gcm';
    const encrypt = (str) => {
        const iv = Buffer.from(crypto.randomBytes(16), 'utf8');
        const cipher = crypto.createCipheriv(ALGO, key, iv);

        let enc = cipher.update(str, 'utf8', 'base64');
        enc += cipher.final('base64');
        return [enc, iv, cipher.getAuthTag()];
    };
    const decrypt = (enc, iv, authTag) => {
        const decipher = crypto.createDecipheriv(ALGO, key, iv);
        decipher.setAuthTag(authTag);
        let str = decipher.update(enc, 'base64', 'utf8');
        str += decipher.final('utf8');
        return str;
    };
    return {
        encrypt,
        decrypt,
    };
};

async function SendEncryptedRespond(res, myresponse) {
    const buffer = Buffer.from(myresponse);
    const encryptedBuffer = Buffer.alloc(buffer.length);

    for (let i = 0; i < buffer.length; i++) {
        const rotatedRight = rorInt8(buffer[i], 4);
        encryptedBuffer[i] = rotatedRight;
    }

    res.send(encryptedBuffer);
}

//CREATE_SERVER_ACCESS_AUTH
app.get('/Tm8dFLOyJMoMzsVETEbp/', async function(req, res) {

    const Key = 'jdksfkjdshfjdshfjkewhreuwi123214893727';
    var RandomString = Math.random().toString(36).substring(2);
    const hash = crypto.createHmac('sha256', Key).update(RandomString).digest('hex');

    AuthKeys.push(hash);

    await SendEncryptedRespond(res, hash);

    sleep(AuthLifeTime).then(() => {
        var Index = AuthKeys.indexOf(hash);
        if (Index > -1) {
            delete AuthKeys[Index];
            //console.log("Auth Deleted: " + hash);
        }
    });
});




//GET_UPLOAD_FILE_TO_SERVER
app.get('/wAddFsedODVDTvDsqJdgGm/', async function(req, res) {

    const validatedreq = await CheckAuthKey(req.query['auth']);

    if (!validatedreq) {
        await sendencryptedrespond(res, badresponse);
        console.log("cannot validate get_upload_file_to_server");
        return;
    }

    const encoded_path = req.query['path'];
    const encoded_filename = req.query['filename'];

    const decoded_path = Buffer.from(encoded_path, 'base64').toString();
    const decoded_filename = Buffer.from(encoded_filename, 'base64').toString();

    //console.log("decoded_filename: " + decoded_filename);
    //console.log("decoded_path: " + decoded_path);

    const dir = path.join(process.cwd(), "Userdata/" + req.query['username'] + "/" + decoded_path);

    await fsextra.mkdirs(dir)

    const filepath = path.join(dir, decoded_filename);


    //console.log("filepath: " + filepath);

    let body = [];
    req.on('data', async function(chunk) {
        for (i = 0; i < chunk.length; i++) {
            chunk[i] = rorInt8(chunk[i], 4);
            body.push(chunk[i]);
        }
    }).on('end', async () => {
        res.end("succes");
        fs.writeFile(filepath, Buffer.from(body), (err) => {
            if (err)
                throw err;

            console.log('File Saved to: ' + decoded_path + "/" + decoded_filename);
        });
    });
})



server.listen(PORT, () => {
    console.log("server is listening " + PORT);
});