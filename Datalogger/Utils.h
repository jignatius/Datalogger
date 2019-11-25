#pragma once

#include <string>

namespace
{
	std::string GenerateJsonTemplate(const std::string& version, const std::string& siteUUID,
		const std::string& masterSN, const std::string dateTime)
	{
		std::string jsonTemplate = "{"\
			"\"version\": \"" + version + "\","\
			"\"site_id\": \"" + siteUUID + "\","\
			"\"master_id\": \"" + masterSN + "\","\
			"\"date_time\": \"" + dateTime + "\","\
			"\"comm_error\": \"0\","\
			"\"unit_info\": {"\
			"\"id\": \"0\","\
			"\"board_sn_1\": \"0\","\
			"\"board_sn_2\": \"0\","\
			"\"board_sn_3\": \"0\","\
			"\"meters_present\": \"0\","\
			"\"firmware_version\": \"0\","\
			"\"configuration\": \"0\","\
			"\"alarm_hardware\": \"0\","
			"\"alarm_comm\": \"0\","\
			"\"dhw_demand\": \"0\","\
			"\"htg_demand\": \"0\","\
			"\"kwf_demand\": \"0\","\
			"\"dhw_flow\": \"0\","\
			"\"dhw_temp\": \"0\","\
			"\"htg_flow\": \"0\","\
			"\"htg_valve_pos\": \"0\","\
			"\"dhw_valve_pos\": \"0\","\
			"\"three_way_valve\": \"0\","\
			"\"htg_return\": \"0\","\
			"\"htg_target_temp\": \"0\","\
			"\"outside_temp\": \"0\","\
			"\"pump\": \"0\","\
			"\"pump_speed\": \"0\","\
			"\"pressure\": \"0\","\
			"\"dhw_setpoint\": \"0\","\
			"\"htg_setpoint\": \"0\","\
			"\"room_temp\": \"0\","\
			"\"date\": \"0\","\
			"\"time\": \"0\","\
			"\"weekday\": \"0\""\
			"},"\
			"\"primary_meter\": {"\
			"\"consumption\": \"0\","\
			"\"flow\": \"0\","\
			"\"flow_temp\": \"0\","\
			"\"total_flow\": \"0\","\
			"\"return_temp\": \"0\","\
			"\"inst_power\": \"0\","\
			"\"serial_number\": \"0\""\
			"},"\
			"\"secondary_meter\": {"""\
			"\"consumption\": \"0\","\
			"\"flow\": \"0\","\
			"\"flow_temp\": \"0\","\
			"\"total_flow\": \"0\","
			"\"return_temp\": \"0\","\
			"\"inst_power\": \"0\","\
			"\"serial_number\": \"0\""\
			"},"\
			"\"pulse_meter_1\": {"""\
			"\"consumption\": \"0\""\
			"},"\
			"\"pulse_meter_2\": {"""\
			"\"consumption\": \"0\""\
			"},"\
			"\"electrical_meter\": {"""\
			"\"consumption\": \"0\","\
			"\"inst_power\": \"0\""\
			"}"\
			"}";

		return jsonTemplate;
	}
}
