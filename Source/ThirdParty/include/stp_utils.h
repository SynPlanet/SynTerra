// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include <glm/glm.hpp>

#ifdef STP_UTILS_EXPORTS
#define STP_UTILS_API __declspec(dllexport)
#else
#define STP_UTILS_API __declspec(dllimport)
#endif


extern "C" {
	STP_UTILS_API void init_GDAL();
	STP_UTILS_API void set_option_GDAL( const char*, const char* );
	STP_UTILS_API void set_search_path_PROJ( const char* );
	STP_UTILS_API void destroy_GDAL();
	STP_UTILS_API const char* get_version_GDAL();

	STP_UTILS_API bool is_valid_email( const char* );
}

struct Box_2d
{
	double x_min = 0;
	double x_max = 0;
	double y_min = 0;
	double y_max = 0;
};

extern "C" {
	STP_UTILS_API void check_active_tiles( double, double, Box_2d**, int& );
	STP_UTILS_API void get_active_tiles( Box_2d**, int& );

	STP_UTILS_API void reset_active_tiles();

	STP_UTILS_API void free_array( const Box_2d* );

	STP_UTILS_API void set_path_resources( const char* );

	STP_UTILS_API bool is_authorized();
}

struct Mesh
{
	glm::dvec3* vertices = nullptr;
	int vsize = 0;
	int* indices = nullptr;
	int isize = 0;
	glm::dvec3* normals = nullptr;
	int nsize = 0;
	glm::dvec2* uvs = nullptr;
	int tsize = 0;
};

typedef void (*cb1i)(void*,int);
typedef void (*cb2i)(void*, int,int);
typedef void (*cb1s)(void*, const char*);
typedef void (*cb1i1s)(void*, int, const char*);
typedef void (*cb1m1s)(void*, const Mesh*, const char*);

extern "C" {
	STP_UTILS_API void free_mesh( const Mesh* );

	STP_UTILS_API void generate_tiles(
		void*,
		cb1i1s,
		cb1i, cb2i,
		bool& should_cancel,
		int cs_mode,
		cb1s, cb1s, cb1m1s,
		cb1s );

	STP_UTILS_API void init_NetSys();
	STP_UTILS_API void destroy_NetSys();

	STP_UTILS_API const char* send_sign_in( const char*, const char* );
	STP_UTILS_API const char* send_sign_up( const char*, const char*, const char* );
}

struct UserData
{
	const char* user_name = nullptr;
	const char* email = nullptr;
	int current_limit = 0;
};

extern "C" {
	STP_UTILS_API UserData* get_user_data();
	STP_UTILS_API void free_string( const char* );
}

