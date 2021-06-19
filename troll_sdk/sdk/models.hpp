#pragma once
#include <cstdint>
#include "math/vector.hpp"
#include "math/matrix.hpp"

class quaternion;
class radian_euler {
public:
	radian_euler( void ) { }
	radian_euler( float x, float y, float z ) { this->x = x; this->y = y; this->z = z; }
	radian_euler( quaternion const& q );
	radian_euler( const vec3_t& angles );

	inline void init( float x = 0.0f, float y = 0.0f, float z = 0.0f ) { this->x = x; this->y = y; this->z = z; }

	vec3_t to_angle( void ) const;
	bool is_valid( ) const;
	void invalidate( );

	inline float* base( ) { return &x; }
	inline const float* base( ) const { return &x; }

	float operator[]( int i ) const;
	float& operator[]( int i );

	float x, y, z;
};

class quaternion
{
public:
	inline quaternion( void ) {}
	inline quaternion( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) { }
	inline quaternion( radian_euler const& angle );

	inline void init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f ) { x = ix; y = iy; z = iz; w = iw; }

	bool is_valid( ) const;
	void invalidate( );

	bool operator==( const quaternion& src ) const;
	bool operator!=( const quaternion& src ) const;

	float* base( ) { return ( float* ) this; }
	const float* base( ) const { return ( float* ) this; }

	// array access...
	float operator[]( int i ) const;
	float& operator[]( int i );

	float x, y, z, w;
};

class __declspec( align( 16 ) ) quaternion_aligned : public quaternion {
public:
	inline quaternion_aligned( void ) {};
	inline quaternion_aligned( float x, float y, float z, float w )
	{
		init( x, y, z, w );
	}

public:
	explicit quaternion_aligned( const quaternion& other )
	{
		init( other.x, other.y, other.z, other.w );
	}

	quaternion_aligned& operator=( const quaternion& other )
	{
		init( other.x, other.y, other.z, other.w );
		return *this;
	}
};

enum bone_flags {
	bone_calculate_mask = 0x1f,
	bone_physically_simulated = 0x01,
	bone_physics_procedural = 0x02,
	bone_always_procedural = 0x04,
	bone_screen_align_sphere = 0x08,
	bone_screen_align_cylinder = 0x10,
	bone_used_mask = 0x0007ff00,
	bone_used_by_anything = 0x0007ff00,
	bone_used_by_hitbox = 0x00000100,
	bone_used_by_attachment = 0x00000200,
	bone_used_by_vertex_mask = 0x0003fc00,
	bone_used_by_vertex_lod0 = 0x00000400,
	bone_used_by_vertex_lod1 = 0x00000800,
	bone_used_by_vertex_lod2 = 0x00001000,
	bone_used_by_vertex_lod3 = 0x00002000,
	bone_used_by_vertex_lod4 = 0x00004000,
	bone_used_by_vertex_lod5 = 0x00008000,
	bone_used_by_vertex_lod6 = 0x00010000,
	bone_used_by_vertex_lod7 = 0x00020000,
	bone_used_by_bone_merge = 0x00040000,
	bone_type_mask = 0x00f00000,
	bone_fixed_alignment = 0x00100000,
	bone_has_saveframe_pos = 0x00200000,
	bone_has_saveframe_rot = 0x00400000
};

enum hitgroups {
	hitgroup_generic = 0,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_leftarm,
	hitgroup_rightarm,
	hitgroup_leftleg,
	hitgroup_rightleg,
	hitgroup_neck,
	hitgroup_gear = 10
};

enum modtypes {
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

enum hitboxes {
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_chest,
	hitbox_lower_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};



struct studio_bone_t {
	int name_index;
	inline char* const name( void ) const {
		return ( ( char* ) this ) + name_index;
	}
	int parent;
	int bone_controller[ 6 ];

	vec3_t pos;
	quaternion quat;
	radian_euler rotation;

	vec3_t pos_scale;
	vec3_t rot_scale;

	matrix_t pose_to_bone;
	quaternion quat_alignement;
	int flags;
	int proc_type;
	int proc_index;
	mutable int physics_bone;

	inline void* procedure( ) const {
		if ( proc_index == 0 ) return NULL;
		else return( void* ) ( ( ( unsigned char* ) this ) + proc_index );
	};

	int surface_prop_idx;
	inline char* const surface_prop( void ) const {
		return ( ( char* ) this ) + surface_prop_idx;
	}
	inline int get_surface_prop( void ) const {
		return surf_prop_lookup;
	}

	int contents;
	int surf_prop_lookup;
	int unused[ 7 ];
};

struct studio_box_t {
	int bone;
	int group;
	vec3_t mins;
	vec3_t maxs;
	int name_index;
	vec3_t angle;
	float radius;
	int pad02[ 4 ];
};

struct studio_hitbox_set_t {
	int name_index;
	int hitbox_count;
	int hitbox_index;

	inline char* const name( void ) const {
		return ( ( char* ) this ) + name_index;
	}
	inline studio_box_t* hitbox( int i ) const {
		return ( studio_box_t* ) ( ( ( unsigned char* ) this ) + hitbox_index ) + i;
	}
};

class studio_hdr_t {
public:
	int id;
	int version;
	long checksum;
	char name_char_array[ 64 ];
	int length;
	vec3_t eye_pos;
	vec3_t illium_pos;
	vec3_t hull_mins;
	vec3_t hull_maxs;
	vec3_t mins;
	vec3_t maxs;
	int flags;
	int bones_count;
	int bone_index;
	int bone_controllers_count;
	int bone_controller_index;
	int hitbox_sets_count;
	int hitbox_set_index;
	int local_anim_count;
	int local_anim_index;
	int local_seq_count;
	int local_seq_index;
	int activity_list_version;
	int events_indexed;
	int textures_count;
	int texture_index;

	studio_hitbox_set_t* hitbox_set( int i ) {
		if ( i > hitbox_sets_count ) return nullptr;
		return ( studio_hitbox_set_t* ) ( ( uint8_t* ) this + hitbox_set_index ) + i;
	}
	studio_bone_t* bone( int i ) {
		if ( i > bones_count ) return nullptr;
		return ( studio_bone_t* ) ( ( uint8_t* ) this + bone_index ) + i;
	}

};

struct model_t {
	void* handle;
	char name[ 260 ];
	int loadFlags;
	int serverCount;
	int type;
	int flags;
	vec3_t  vecMins;
	vec3_t  vecMaxs;
	float   radius;
};

struct model_render_info_t {
	vec3_t origin;
	vec3_t angles;
	char pad[ 0x4 ]; // added this
	void* renderable; // this
	//const void *model; // and this
	const model_t* model;
	const matrix_t* model_to_world;
	const matrix_t* lighting_offset;
	const vec3_t* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	model_render_info_t( ) {
		model_to_world = NULL;
		lighting_offset = NULL;
		lighting_origin = NULL;
	}
};
struct studiohwdata_t;
typedef void* studio_decal_handle_t;
class i_client_renderable;
struct draw_model_state_t {
	studio_hdr_t* studio_hdr;
	studiohwdata_t* studio_hwdara;
	i_client_renderable* renderable;
	const matrix_t* model_to_world;
	studio_decal_handle_t     m_decals;
	int                     m_drawflags;
	int                     m_lod;
};

struct renderable_info_t {
	i_client_renderable* renderable;
	void* alpha_property;
	int enum_count;
	int render_frame;
	unsigned short first_shadow;
	unsigned short leaf_list;
	short area;
	uint16_t flags;
	uint16_t flags2;
	vec3_t bloated_abs_mins;
	vec3_t bloated_abs_maxs;
	vec3_t abs_mins;
	vec3_t abs_maxs;
	int pad;
};
