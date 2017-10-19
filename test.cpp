#include "xxh64.hpp"
#include <cstdio>
#include <cinttypes>
#include <type_traits>
#include <array>

bool checkhash (const void *ptr, int len, uint64_t seed, uint64_t expected)
{
    uint64_t hash = xxh64::hash (reinterpret_cast<const char*> (ptr), len, seed);
    if (hash != expected) {
        printf("Test failed: 0x%llx!= 0x%llx",hash,expected);
        return false;
    }
    return true;
}

int main (int argc, char *argv[])
{
    //calculate xxhast at compile time
    constexpr const char    aString[]     = "Does a set of all sets contain itself?";
    constexpr const char    bString[]     = "There is nothing either good or bad, but thinking makes it so.";
    constexpr const char    cString[]     = "I would give all my fame for a pot of ale, and safety.";

    
    
    // Print the hash we computed for the string (in the compiler)
    printf( "Hash of \"%s\" = %llx\n",aString , std::integral_constant<uint64_t,xxh64::hash(aString)>::value);
    printf( "Hash of \"%s\" = %llx\n",bString , std::integral_constant<uint64_t,xxh64::hash(bString)>::value);
    printf( "Hash of \"%s\" = %llx\n",cString , std::integral_constant<uint64_t,xxh64::hash(cString)>::value);

    
    
    
    //calculate xxhash at runtime
    std::array<uint8_t, 101> data;
    static constexpr uint32_t prime = 2654435761U;

    uint32_t byteGen = prime;
    for (auto i = 0; i < data.size (); i++) {
        data[i] = byteGen >> 24;
        byteGen *= byteGen;
    }

    bool result = true;

    result &= checkhash (nullptr, 0, 0, 0xEF46DB3751D8E999ULL);
    result &= checkhash (nullptr, 0, prime, 0xAC75FDA2929B17EFULL);
    result &= checkhash (data.data (), 1, 0, 0x4FCE394CC88952D8ULL);
    result &= checkhash (data.data (), 1, prime, 0x739840CB819FA723ULL);
    result &= checkhash (data.data (), 14, 0, 0xCFFA8DB881BC3A3DULL);
    result &= checkhash (data.data (), 14, prime, 0x5B9611585EFCC9CBULL);
    result &= checkhash (data.data (), data.size (), 0, 0x0EAB543384F878ADULL);
    result &= checkhash (data.data (), data.size (), prime, 0xCAA65939306F1E21ULL);

    printf(result ? "All tests passed successfully.\n":"Test failed\n");
    
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
