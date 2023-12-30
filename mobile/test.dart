import 'dart:io';
import 'dart:typed_data';
import 'dart:convert';
import 'dart:math';


void main() async{

final imageFile = File("/home/ardakilic/Desktop/CSE396/mobile/received_image.jpg");
Uint8List imageBytes = imageFile.readAsBytesSync();
print("Length in bytes:");
print(imageBytes.lengthInBytes);

}
