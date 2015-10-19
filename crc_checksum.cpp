
inline
unsigned short checksum(const char *buf)
{
    unsigned int count = strlen(buf);
    unsigned char *addr = (unsigned char*)buf;
    register unsigned int sum = 0;

    // Main summing loop
    while(count > 1)
    {
        // sum = sum + *((unsigned short *) addr)++;
        sum = sum + *((unsigned short *) addr);
        addr += 2;
        count -= 2;
    }

    // Add left-over byte, if any
    if (count > 0)
        sum = sum + *((unsigned char *) addr);

    // Fold 32-bit sum to 16 bits
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return(~sum);
}

