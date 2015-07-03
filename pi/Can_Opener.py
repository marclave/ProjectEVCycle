import struct

class Can_Opener(object):
	def __init__(self):
		#super(Can_Opener, self).__init__()
		print("INIT")

	def pack_data(self, can_byte_array):
		buf = bytes()
		buf = buf.join((struct.pack('B', val) for val in can_byte_array))
		packed_can_data = int.from_bytes(buf, byteorder='big')
		return packed_can_data
    
	def shift_mask(self, start_loc, length, lumped_data, mask):
		shift = 64 - start_loc - length
		lumped_data = lumped_data>>shift
		lumped_data = lumped_data & mask
		return lumped_data

#Just some testing
# test = Can_opener
# data = [0x31,0x36,0xB0,0xac,0x13,0x6B,0x0a,0x07]
# data_bytes = test.pack_data(data)
# print(test.shift_mask(8,16,data_bytes,SIXTEEN_BIT_MASK))
# print(test.shift_mask(36,16,data_bytes,SIXTEEN_BIT_MASK))