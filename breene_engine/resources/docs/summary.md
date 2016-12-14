http://ogldev.atspace.co.uk/www/tutorial30/tutorial30.html
http://ogldev.atspace.co.uk/www/tutorial30/tutorial31.html
---
Basic runthrough:
• The VS is executed on every vertex in a patch. The patch comprises several CPs from the vertex buffer (up to a limit defined by the driver and GPU).
• The TCS takes the vertices that have been processed by the VS and generates an output patch. In addition, it generates TLs.
• Based on the configured domain, the TLs it got from the TCS and the configured output topology, the PG generates domain location and their connectivity.
• The TES is executed on all generated domain locations.
• The primitives that were generated in step 3 continue down the pipe. The output from the TES is their data.
• Processing continues either at the GS or at the rasterizer.
---
