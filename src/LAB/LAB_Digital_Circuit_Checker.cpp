#include "LAB_Digital_Circuit_Checker.h"

#include <fstream>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "LAB.h"

LAB_Digital_Circuit_Checker::
LAB_Digital_Circuit_Checker (LAB_Core* _LAB_Core, LAB* _LAB)
  : m_LAB_Core    (_LAB_Core), 
    m_LAB         (_LAB),
    m_hw_expander (LABC::PIN::DCC_XPAND_CS,
                  LABC::PIN::DCC_XPAND_MISO,
                  LABC::PIN::DCC_XPAND_MOSI,
                  LABC::PIN::DCC_XPAND_SCLK,
                  LABC::DIGITAL_CIRCUIT_CHECKER::IC_FREQUENCY,
                  0)
{
  init_hw_expander ();
}

LAB_Digital_Circuit_Checker:: 
~LAB_Digital_Circuit_Checker ()
{
  if (m_is_file_loaded)
  {
    release_file_lock (m_file_path);
  }
}

void LAB_Digital_Circuit_Checker::
init_hw_expander ()
{
  m_hw_expander.direction (LABC::DIGITAL_CIRCUIT_CHECKER::INPUT_PORT,
    LAB_MCP23S17::DIRECTION::INPUT);

  m_hw_expander.direction (LABC::DIGITAL_CIRCUIT_CHECKER::OUTPUT_PORT,
    LAB_MCP23S17::DIRECTION::OUTPUT);
}

bool LAB_Digital_Circuit_Checker:: 
acquire_file_lock  (const std::string& path)
{
  if (chmod (path.c_str (), S_IRUSR | S_IRGRP | S_IROTH) == 0)
  {
    return (true);
  }
  else 
  {
    throw (std::runtime_error ("Unable to lock LAB Circuit Checker file."));

    return (false);
  }
}

bool LAB_Digital_Circuit_Checker:: 
release_file_lock  (const std::string& path)
{
  if (chmod (path.c_str (), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) == 0)
  {
    return (true);
  }
  else 
  {
    throw (std::runtime_error ("Unable to unlock LAB Circuit Checker file."));

    return (false);
  }
}

void LAB_Digital_Circuit_Checker:: 
clear_vectors ()
{
  m_inputs              .clear ();
  m_outputs             .clear ();
  m_actual_outputs      .clear ();
  m_char_inputs         .clear ();
  m_char_outputs        .clear ();
  m_char_actual_outputs .clear ();
}

void LAB_Digital_Circuit_Checker:: 
load_metadata ()
{
  pugi::xml_node metadata = m_xml_doc.child ("root").child ("metadata");

  m_input_bits  = metadata.child ("input_bits").text ().as_int ();
  m_output_bits = metadata.child ("output_bits").text ().as_int ();
}

void LAB_Digital_Circuit_Checker:: 
load_data_pairs ()
{
  if (!m_is_file_loaded)
  {
    return;
  }

  //

  pugi::xml_node data = m_xml_doc.child ("root").child ("data");

  for (const pugi::xml_node& data_pair : data)
  {
    std::string raw;

    raw = data_pair.child_value ("input");
    m_char_inputs.emplace_back (std::vector<char> (raw.begin (), raw.end ()));
    std::replace (raw.begin (), raw.end (), 'X', '0');    
    m_inputs.emplace_back (std::stoi (raw));

    raw = data_pair.child_value ("output");
    m_char_outputs.emplace_back (std::vector<char> (raw.begin (), raw.end ()));
    std::replace (raw.begin (), raw.end (), 'X', '0');    
    m_outputs.emplace_back (std::stoi (raw));
  }
}

void LAB_Digital_Circuit_Checker:: 
load_data_from_file ()
{
  clear_vectors   ();
  load_metadata   ();
  load_data_pairs ();
}

void LAB_Digital_Circuit_Checker:: 
load_file (const std::string& path)
{
  // std::string decrypted_xml = decrypt_xml (path);
  // pugi::xml_parse_result res = m_xml_doc.load_string (decrypted_xml.c_str ());

  pugi::xml_document doc;

  pugi::xml_parse_result res = doc.load_file (path.c_str ());

  if (!res)
  {
    unload_file ();

    throw (std::domain_error ("Invalid or corrupted LAB Circuit Checker file."));
  }
  else 
  {
    m_file_path = path;

    acquire_file_lock (m_file_path);

    m_is_file_loaded = true;
  }
}

void LAB_Digital_Circuit_Checker:: 
unload_file ()
{
  if (m_is_file_loaded)
  {
    release_file_lock (m_file_path);

    m_file_path = "";

    m_is_file_loaded = false;
  }
}

void LAB_Digital_Circuit_Checker:: 
perform_check ()
{
  m_actual_outputs.resize (m_outputs.size ());

  for (size_t i = 0; i < m_inputs.size (); i++)
  {
    m_hw_expander.write (LABC::DIGITAL_CIRCUIT_CHECKER::INPUT_PORT, m_inputs[i]);

    std::this_thread::sleep_for (std::chrono::milliseconds (100));

    m_actual_outputs[i] = m_hw_expander.read (LABC::DIGITAL_CIRCUIT_CHECKER::OUTPUT_PORT);

    std::this_thread::sleep_for (std::chrono::milliseconds (100));
  }
}

void LAB_Digital_Circuit_Checker:: 
calculate_scores ()
{
  m_score_total   = m_inputs.size ();
  m_score_current = 0;

  for (size_t i = 0; i < m_outputs.size (); ++i)
  {
    if (m_actual_outputs[i] == m_outputs[i])
    {
      m_score_current++;
    }
  }

  m_score_percent = (static_cast<double>(m_score_current) / m_score_total) * 100.0;
}

void LAB_Digital_Circuit_Checker:: 
generate_char_actual_outputs_vec ()
{
  m_char_actual_outputs.resize (m_char_outputs.size ());

  for (size_t row = 0; row < m_char_actual_outputs.size (); row++)
  {
    std::vector<char> vec;
    uint8_t           row_val = m_actual_outputs[row];

    for (int a = 7; a >= 0; a--)
    {
      char c = ((row_val >> a) & 1) ? '1' : '0';
    
      vec.push_back (c);
    }

    m_char_actual_outputs[row] = vec;
  }
}

std::string LAB_Digital_Circuit_Checker:: 
decrypt_xml (const std::string& path)
{
  std::string key ("thequickbrownfox");

  // Load the encrypted XML file into memory
    std::ifstream encryptedFile(path, std::ios::binary);
    std::stringstream buffer;
    buffer << encryptedFile.rdbuf();
    std::string encryptedData = buffer.str();

  const int AES_KEY_SIZE = 256; // 256-bit key size for AES-256

  // Set up the encryption key and initialization vector (IV)
  unsigned char iv[AES_BLOCK_SIZE];
  memset(iv, 0, sizeof(iv));

  // Set up the encryption context
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), iv);

  // Determine the output buffer size
  size_t decryptedSize = encryptedData.size();
  std::string decryptedData(decryptedSize, '\0');

  // Decrypt the encrypted data
  int bytesDecrypted = 0;
  EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&decryptedData[0]), &bytesDecrypted, reinterpret_cast<const unsigned char*>(encryptedData.c_str()), decryptedSize);

  int finalBytesDecrypted = 0;
  EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&decryptedData[bytesDecrypted]), &finalBytesDecrypted);

  // Resize the decrypted data to the actual size
  decryptedData.resize(bytesDecrypted + finalBytesDecrypted);

  // Clean up the encryption context
  EVP_CIPHER_CTX_free(ctx);

  std::cout << decryptedData << "\n";

    return decryptedData;
}

void LAB_Digital_Circuit_Checker:: 
run_checker ()
{
  if (!m_is_file_loaded)
  {
    throw (std::runtime_error ("No LAB Circuit Checker file loaded."));

    return;
  }

  //

  m_are_results_ready = false;

  m_xml_doc.load_file               (m_file_path.c_str ());
  load_data_from_file               (); 
  perform_check                     ();
  calculate_scores                  ();
  generate_char_actual_outputs_vec  ();

  m_are_results_ready = true;
}

LAB_Digital_Circuit_Checker::ScoreData LAB_Digital_Circuit_Checker:: 
get_score_data () const
{
  LAB_Digital_Circuit_Checker::ScoreData score_data = 
  {
    .input_bits           = m_input_bits,
    .output_bits          = m_output_bits,

    .score_current        = m_score_current,
    .score_total          = m_score_total,
    .score_percent        = m_score_percent,

    .inputs               = m_inputs,
    .outputs              = m_outputs,
    .actual_outputs       = m_actual_outputs,

    .char_inputs          = m_char_inputs,
    .char_outputs         = m_char_outputs,
    .char_actual_outputs  = m_char_actual_outputs
  };

  return (score_data);
}

bool LAB_Digital_Circuit_Checker:: 
are_results_ready () const
{
  return (m_are_results_ready);
}

// EOF 
