function Decoder(bytes, port) {
 var result = "";
 for (var i = 0; i < bytes.length; i++) {
   result += String.fromCharCode(parseInt(bytes[i]));
 }
 return { cpt: result, };
}