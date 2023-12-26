import 'dart:io';
import 'dart:typed_data';
import 'dart:convert';
import 'dart:math';

void main() async{
  String imagePath = 'received_image.jpg';
  File imageFile = File(imagePath);
  final chunkSize = 4095;
  int bytesWritten = 0;
  Uint8List extractedBytes = Uint8List(0);
  String messageToSend = "";
  Uint8List imageBytes = Uint8List(0);
  print("test1");

  final socket = await Socket.connect("127.0.0.1", 8080);

  messageToSend = "0Connection EstablishedMOBILE";
  socket.write(messageToSend);
  print("Mobile response: ${(messageToSend)[0]}");


  int i = 0;
  Uint8List chunkWithPrefix = Uint8List.fromList([]);

  bool draw = false;

    // Function to set draw to true after 2 seconds
  Future<void> setDrawToTrue() async {
    await Future.delayed(const Duration(seconds: 2));
    draw = true;
    print('Draw set to true after 2 seconds.');
  }

  // Start the timer to set draw to true after 2 seconds
  setDrawToTrue();
  

  socket.listen(
    (Uint8List data) {
      print('Sunucudan gelen veri: ${String.fromCharCodes(data)}');
      final serverResponse = Uint8List.fromList(data)[0] - 48;
      print("gelen komut -> $serverResponse");
      messageToSend = "";
      

      if (serverResponse == 0) {

        if(draw){
          final imageFile = File(imagePath);
          imageBytes = imageFile.readAsBytesSync();
          print("Length in bytes:");
          print(imageBytes.lengthInBytes);
          print("bytesWritten => $bytesWritten, imageBytes.lengthInBytes => ${imageBytes.lengthInBytes}");
          if(bytesWritten < imageBytes.lengthInBytes){
              print("BYTES SEND");
              int remainingBytes = imageBytes.lengthInBytes - bytesWritten;
              int bytesToWrite = min(chunkSize, remainingBytes);
              extractedBytes = imageBytes.sublist(bytesWritten, bytesWritten + bytesToWrite);
              bytesWritten += bytesToWrite;
              print('Bytes sent: $bytesToWrite');
          }

          if(bytesWritten >= imageBytes.lengthInBytes){
              print("LAST BYTES SEND");
              messageToSend = '2' + String.fromCharCodes(extractedBytes);
              bytesWritten = 0;
              imageBytes.clear();
              draw = false;
          }
          else{
            messageToSend = '1' + String.fromCharCodes(extractedBytes);
          }
        }
        else{
          print("draw is false\n");
          messageToSend =  '0' + "Connection EstablishedMOBILE";
        }
      }
      else if (serverResponse == 1) {
        print("bytesWritten => $bytesWritten, imageBytes.lengthInBytes => ${imageBytes.lengthInBytes}");
        if(bytesWritten < imageBytes.lengthInBytes){
          print("BYTES SEND");
          int remainingBytes = imageBytes.lengthInBytes - bytesWritten;
          int bytesToWrite = min(chunkSize, remainingBytes);
          extractedBytes = imageBytes.sublist(bytesWritten, bytesWritten + bytesToWrite);
          bytesWritten += bytesToWrite;
          print('Bytes sent: $bytesToWrite');
        }

        if(bytesWritten >= imageBytes.lengthInBytes){
          print("LAST BYTES SEND");
          messageToSend = '2' + String.fromCharCodes(extractedBytes);
          bytesWritten = 0;
          imageBytes.clear();
          draw = false;
        }
        else{
          messageToSend = '1' + String.fromCharCodes(extractedBytes);
        }
      }
      else if (serverResponse == 2) {
        messageToSend =  '6' + "Connection Established";
      }
      else if (serverResponse == 3) {
        messageToSend =  '6' + "Connection Established";
      }
      else if (serverResponse == 4) {
        messageToSend =  '6' + "Connection Established";
      }
      else if (serverResponse == 5) {
        messageToSend =  '6' + "Connection Established";
      }
      else if (serverResponse == 6) {
        messageToSend =  '6' + "Connection Established";
      }

      if(!messageToSend.isEmpty && !data.isEmpty){
          socket.write(messageToSend);
          final mobilResponse = (messageToSend)[0];
          print("mobil response -> ${(messageToSend)[0]}");
          //print(mobilResponse);
      }
      sleep(Duration(milliseconds:100));
    },
    onError: (error) {
      print('Error: $error');
    },
    onDone: () {
      print('Server closed connection.');
    },
  );

}