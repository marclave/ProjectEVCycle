import can
import CAN_Opener

can.rc['interface'] = 'socketcan_native'
from can.interfaces.interface import Bus
can_interface = "can0"


CAN_MASK = 0x700

CAN_MESSAGE_1 = 0x100
CAN_MESSAGE_2 = 0x200
CAN_MESSAGE_3 = 0x300
CAN_MESSAGE_4 = 0x400

ENTRY_1 = {'can_id':CAN_MESSAGE_1, 'can_mask':CAN_MASK}
ENTRY_2 = {'can_id':CAN_MESSAGE_2, 'can_mask':CAN_MASK}
ENTRY_3 = {'can_id':CAN_MESSAGE_3, 'can_mask':CAN_MASK}
ENTRY_4 = {'can_id':CAN_MESSAGE_4, 'can_mask':CAN_MASK}

ONE_BIT_MASK        = 0x1
TWO_BIT_MASK        = 0X3
THREE_BIT_MASK      = 0x7
FOUR_BIT_MASK       = 0xF

FIVE_BIT_MASK       = 0x1F
SIX_BIT_MASK        = 0x3F
SEVEN_BIT_MASK      = 0x7F
EIGHT_BIT_MASK      = 0xFF

NINE_BIT_MASK       = 0x1FF
TEN_BIT_MASK        = 0x3FF
ELEVEN_BIT_MASK     = 0x7FF
TWELVE_BIT_MASK     = 0xFFF

THRIRTEEN_BIT_MASK  = 0x1FFF
FOURTEEN_BIT_MASK   = 0x3FFF
FIFTEEN_BIT_MASK    = 0x7FFF
SIXTEEN_BIT_MASK    = 0xFFFF


class CAN_Main(object):
	def __init__(self):

		self.currentSpeed = 0
		self.previousSpeed = 0
		self.updateSpeed = False

		self.currentVoltage = 0
		self.previousVoltage = 0
		self.updateVoltage = False

		self.leftBlindSpot = False
		self.leftBottomBlindSpot = False
		self.centerBottombBlindSpot = False
		self.rightBottomBlindSpot = False
		self.rightBlindSpot = False

		self.overVoltageWarning = False
		self.underVoltageWarning = False
		
		self.overTemperatureWarning = False
		self.underTemperatureWarning = False

		self.overCurrentWarning = False
		self.underCurrentWarning = False

	def initializeInstances(self):
		self.bus = Bus(can_interface,can_filters=FILTER_DICTIONARY_LIST)
		self.can_tools = CAN_Opener.Can_Opener()

	def pollBus(self):
		try:
			msg = self.bus.recv(timeout=10)
			self.process_CAN_message(msg)
		except:
			print("TODO m: We need to catch this, yo")
			raise

	def process_CAN_message(self, pCan_frame):
		self.message_select(pCan_frame)


	def message_select(self, pCAN_frame):
		if(pCAN_frame.arbitration_id == 0x100):
			self.message_one(pCAN_frame.data)
		elif(pCAN_frame.arbitration_id == 0x200):
			self.message_two(pCAN_frame.data)
		elif(pCAN_frame.arbitration_id == 0x300):
			self.message_three(pCAN_frame.data)
		elif(pCAN_frame.arbitration_id == 0x400):
			self.message_four(pCAN_frame.data)
		else:
			pass

	def shiftData(self, pValue, pShiftPlaces):
		return pValue>>pShiftPlaces

	def message_one(self, data): #Engine Signals
		msg_one_bits = self.can_tools.pack_data(data)
		'''
		TODO m: update to EV
		self.set_engine_coolant_temp(self.shiftData(data[0], 1))
		self.set_engine_torque(data[1])
		self.set_engine_RPM(data[3]*256 + data[2])
		self.set_throttle_percent(self.shiftData(data[4], 1))
		'''

	def message_two(self, data): #Warnings
		msg_two_bits = self.can_tools.pack_data(data)
		'''
		TODO m: update to EV
		self.set_warning_ess_overtemp(self.can_tools.shift_mask(0, 1, msg_two_bits, ONE_BIT_MASK))
		self.set_warning_fuel_level_low(self.can_tools.shift_mask(1, 1, msg_two_bits, ONE_BIT_MASK))
		self.set_warning_glv_cockpit_brb(self.can_tools.shift_mask(2, 1, msg_two_bits, ONE_BIT_MASK))
		self.set_warning_glv_soc_low(self.can_tools.shift_mask(3, 1, msg_two_bits, ONE_BIT_MASK))
		self.set_warning_motor_over_temp(self.can_tools.shift_mask(4, 1, msg_two_bits, ONE_BIT_MASK))
		self.set_warning_transmission_failure(self.can_tools.shift_mask(5, 1, msg_two_bits, ONE_BIT_MASK))
		'''
	
	def message_three(self, data): #Electrical Systems
		'''
		TODO m: update for EV
		self.set_ess_soc(self.shiftData(data[0], 1))
		self.set_ess_voltage(data[1])
		'''

	def message_four(self, data): #Control
		msg_four_bits = self.can_tools.pack_data(data)
		'''
		TODO m: update to EV
		self.set_current_control_mode(self.can_tools.shift_mask(0, 2, msg_four_bits, TWO_BIT_MASK))
		self.set_current_gear(self.can_tools.shift_mask(2, 4, msg_four_bits, FOUR_BIT_MASK))
		self.set_vehicle_speed(data[1])
		self.set_engery_budget_status(data[3])
		self.set_odometer(data[4])
		'''
