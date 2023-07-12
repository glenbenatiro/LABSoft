#include "LABChecker_Main.h"

#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <fstream>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>

unsigned LABChecker_Main:: 
binary_vector_to_decimal (const std::vector<int>& bin_vec)
{
  unsigned decimal  = 0;
  unsigned base     = 1;

  for (std::vector<int>::const_iterator it = bin_vec.begin ();
    it != bin_vec.end (); ++it)
  {
    decimal += (*it) * base;
    base *= 2;
  }
  
  return (decimal);
}

void LABChecker_Main:: 
save_xml_file (       pugi::xml_document& doc,
               const  std::string&        save_path)
{
  std::string extension = ".labcc";
  std::string path      = save_path;

  if (save_path.length () < extension.length () || save_path.compare (
    save_path.length () - extension.length (), extension.length (),
    extension) != 0)
  {
    path += extension;
  }

  encrypt_and_save (doc, path);

  // doc.save_file (path.c_str ());
}

void LABChecker_Main:: 
encrypt_and_save (      pugi::xml_document& doc, 
                  const std::string&        save_path)
{
  // 1. Convert the XML document to a string representation
  std::ostringstream oss;
  doc.save (oss);
  std::string xml_string = oss.str ();

  // 2. Set up the encryption key and initialization vector (IV)
  unsigned char iv[AES_BLOCK_SIZE];

  // Generate a random IV
  if (RAND_bytes (iv, sizeof (iv)) != 1)
  {
    throw (std::runtime_error ("Failed to generate initialization vector."));
  }

  // 3. Encrypt the XML string using AES encryption

  // Set up the encryption context
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new ();

  EVP_EncryptInit_ex (
    ctx, 
    EVP_aes_256_cbc (), 
    NULL, 
    reinterpret_cast<const unsigned char*>(KEY.c_str ()), 
    iv
  );

  // Determine the output buffer size
  size_t encrypted_size = xml_string.size () + AES_BLOCK_SIZE;
  std::string encrypted_data (encrypted_size, '\0');

  // Encrypt the XML string
  int bytes_encrypted = 0;
  EVP_EncryptUpdate (
    ctx, 
    reinterpret_cast<unsigned char*>(&encrypted_data[0]),
    &bytes_encrypted,
    reinterpret_cast<const unsigned char*>(xml_string.c_str ()),
    xml_string.size ()
  );

  int final_encrypted_bytes = 0;
  EVP_EncryptFinal_ex (
    ctx, 
    reinterpret_cast<unsigned char*>(&encrypted_data[bytes_encrypted]),
    &final_encrypted_bytes
  );
  
  // Resize the encrypted data to the acual size
  encrypted_data.resize (bytes_encrypted + final_encrypted_bytes);

  // Clean up the encryption context
  EVP_CIPHER_CTX_free (ctx);

  // 4. Save the encryped data to a new file
  std::ofstream output_file (save_path, std::ios::binary);

  if (output_file) 
  {
    output_file.write (encrypted_data.c_str (), encrypted_data.size ());
    output_file.close ();
  }
  else 
  {
    throw (std::runtime_error ("Failed to create encrypted file."));
  }
}

void LABChecker_Main:: 
create_file_digital (const std::vector<std::vector<char>>& inputs,
                     const std::vector<std::vector<char>>& outputs,
                     const std::string&                    save_path)
{
  pugi::xml_document  doc;
  pugi::xml_node      root = doc.append_child ("root");

  //

  pugi::xml_node metadata = root.append_child ("metadata");

  metadata.append_child ("input_bits")
          .append_child (pugi::node_pcdata)
          .set_value (std::to_string (inputs[0].size ()).c_str ());
  
  metadata.append_child ("output_bits")
          .append_child (pugi::node_pcdata)
          .set_value (std::to_string (outputs[0].size ()).c_str ());

  //

  pugi::xml_node data = root.append_child ("data");

  for (size_t row = 0; row < inputs.size (); ++row)
  {
    // 1. Node
    pugi::xml_node data_pair = data.append_child ("data_pair");
  
    // 2. Input   
    pugi::xml_node input_node = data_pair.append_child ("input");

    input_node.append_child (pugi::node_pcdata).set_value (std::string 
      (inputs[row].begin (), inputs[row].end ()).c_str ());

    // 3. Output
    pugi::xml_node output_node = data_pair.append_child ("output");

    output_node.append_child (pugi::node_pcdata).set_value (std::string 
      (outputs[row].begin (), outputs[row].end ()).c_str ());  
  }  

  save_xml_file (doc, save_path);
}

// EOF
