#pragma once

namespace antiaim {
	inline float m_max_delta;

	/* lby */
	void predict_lby( );
	inline float m_lby_update_time;
	inline bool m_in_lby_update;
	inline bool m_in_balance_update;
	inline bool m_can_micro_move;
	inline bool m_should_resync;
}