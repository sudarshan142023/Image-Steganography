# Image-Steganography

An **Image Steganography** application developed in **C** that securely hides and extracts secret data inside **BMP images** using the **Least Significant Bit (LSB)** algorithm. The project demonstrates file handling, bit manipulation, dynamic memory management, and modular programming to achieve secure data concealment without visibly altering the image.

---

## 📖 Overview

Image Steganography is the technique of hiding confidential information inside a digital image so that the existence of the hidden data is imperceptible. This project uses the **Least Significant Bit (LSB)** method to embed secret text files into 24-bit BMP images and later retrieve the hidden information.

---

## ✨ Features

- 🔒 Hide a secret text file inside a BMP image
- 🔓 Extract the hidden data from the encoded image
- 🖼 Supports 24-bit BMP images
- 📄 Supports any text file as secret data
- 📏 Verifies image capacity before encoding
- 🛡 Uses a magic string for data validation
- 💾 Preserves image quality after encoding
- ⚡ Efficient bitwise operations for embedding and extraction

---

## 🛠 Technologies Used

- C Programming
- GCC Compiler
- Linux
- File Handling
- Bit Manipulation
- Dynamic Memory Allocation
- Modular Programming

---

## 📂 Project Structure

```
.
├── main.c
├── encode.c
├── decode.c
├── common.c
├── common.h
├── encode.h
├── decode.h
├── types.h
├── test_encode.c
├── test_decode.c
├── Makefile
├── beautiful.bmp
├── secret.txt
├── stego.bmp
└── README.md
```

---

## ⚙️ Working Principle

### Encoding

1. Read the source BMP image.
2. Verify image capacity.
3. Copy the BMP header.
4. Embed the magic string.
5. Embed the secret file extension.
6. Embed the secret file size.
7. Embed the secret file data.
8. Copy the remaining image bytes.
9. Generate the encoded BMP image.

---

### Decoding

1. Open the encoded image.
2. Skip the BMP header.
3. Decode the magic string.
4. Verify the magic string.
5. Decode the file extension.
6. Decode the secret file size.
7. Decode the hidden data.
8. Create the original secret file.

---

## 🔐 LSB Algorithm

Each byte of the secret file is embedded into the least significant bits of eight image bytes.

Example

```
Image Byte : 11010010
Secret Bit :        1

Result     : 11010011
```

Only the least significant bit changes, making the modification visually undetectable.

---

## 🚀 Compilation

Using GCC

```bash
gcc *.c -o steganography
```

or

```bash
make
```

---

## ▶️ Execution

### Encoding

```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

### Decoding

```bash
./steganography -d stego.bmp output.txt
```

---

## 📷 Sample Output

### Encoding

```
Encoding Started...

Checking image capacity...
Image capacity verified.

Encoding Magic String...
Encoding Secret File Extension...
Encoding Secret File Size...
Encoding Secret Data...

Encoding Completed Successfully.
```

### Decoding

```
Decoding Started...

Magic String Verified.

Decoding File Extension...
Decoding File Size...
Decoding Secret Data...

Decoding Completed Successfully.
```

---

## 📁 File Flow

```
Source Image (BMP)
        │
        ▼
Embed Secret File
        │
        ▼
Encoded Image (stego.bmp)
        │
        ▼
Decode
        │
        ▼
Recovered Secret File
```

---

## ⏱ Time Complexity

| Operation | Complexity |
|------------|------------|
| Capacity Check | O(n) |
| Encoding | O(n) |
| Decoding | O(n) |
| File Copy | O(n) |

---

## 📚 Concepts Used

- Bit Manipulation
- Least Significant Bit (LSB)
- Binary File Processing
- File Handling
- Dynamic Memory Allocation
- Structures
- Modular Programming

---

## 🎯 Learning Outcomes

This project helped me understand:

- Image Steganography
- Binary file manipulation
- Bitwise operators
- BMP image format
- Dynamic memory allocation
- Modular software design
- Secure data hiding techniques
- File pointer management

---

## 🔮 Future Enhancements

- Support PNG images
- Encrypt data before embedding
- Password-protected decoding
- Hide multiple files
- Audio and video steganography
- GUI version
- AES encryption integration
- Compression before embedding

---

## 👨‍💻 Author

**Sudarshan Jadhav**

- 📧 Email: jadhavsudarshan470@gmail.com
- 💻 GitHub: https://github.com/sudarshan142023
- 🔗 LinkedIn: https://www.linkedin.com/in/sudarshan-jadhav14/

---

## ⭐ If you found this project useful, don't forget to star the repository!
