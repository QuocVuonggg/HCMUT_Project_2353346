#include "../unit_test_hash.hpp"

bool UNIT_TEST_Hash::hash10() {
  string name = "hash10";
  //! data ------------------------------------
  xMap<int, string> hash(xMap<int, string>::intKeyHash, 0.6);
  hash.put(1, "votien1");
  hash.put(11, "votien11");
  hash.put(21, "votien21");
  hash.put(31, "votien31");
  hash.put(41, "votien41");
  hash.put(51, "votien51");
  hash.put(61, "votien61");
  hash.put(2, "votien2");
  hash.put(3, "votien3");
  hash.put(1, "votien_1");

  //! expect ----------------------------------
  string expect = "votien_1";

  //! output ----------------------------------
  stringstream output;
  try {
    output << hash.get(1);
  } catch (const KeyNotFound& e) {
    output << "Caught exception: " << e.what() << std::endl;
  }

  //! remove data -----------------------------

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
