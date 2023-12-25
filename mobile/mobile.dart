import 'dart:io';

void main() {
  String imagePath = '/tmp/cse396/image.jpg';
  File imageFile = File(imagePath);

  if (!imageFile.existsSync()) {
    print('File not found!');
    return;
  }

  List<int> bytes = imageFile.readAsBytesSync();

  // Establish a TCP connection to the server
  Socket.connect('127.0.0.1', 8080).then((socket) {
    print('Connected to server!');

    int packetSize = 4095;
    int totalPackets = (bytes.length / packetSize).ceil();

    for (int i = 0; i < totalPackets; i++) {
      int start = i * packetSize;
      int end = (i == totalPackets - 1) ? bytes.length : (i + 1) * packetSize;

      List<int> packet = bytes.sublist(start, end);
      bool isLastPacket = (i == totalPackets - 1);

      // Add '1' for intermediate packets, '2' for the last packet
      int encoderChar = isLastPacket ? 2 : 1;

      // Create a new packet with the encoder character at the beginning
      List<int> encodedPacket = [encoderChar, ...packet];

      print('Sent package ${i + 1}: $encodedPacket');

      // Send the encoded packet over the socket
      socket.add(encodedPacket);
    }

    // Close the socket after sending the image
    socket.close();
  }).catchError((error) {
    print('Error: $error');
  });
}