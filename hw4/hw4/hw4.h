uint16_t mcp4902_spi_generator(float voltage, bool channel);
static void spi_write_16b(uint16_t data);
static inline void cs_select();
static inline void cs_deselect();