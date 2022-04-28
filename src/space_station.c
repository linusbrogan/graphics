		// Build the planet
		double planet_radius = 10000;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 57;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = -38;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 12000;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 6500;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 12000;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		graph_3d(sphere_xyz, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, M, earth_map);

		// Build the space station
		double half_height = 0.5;
		// Make the space station transformation sequence
		double S[4][4];
		T_n = 0;
		T_type[T_n] = RX;	T_param[T_n] = 15;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(S, _i, T_n, T_type, T_param);
		M3d_mat_mult(S, view, S);

		// Build the upper ring
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_ring_xyz, 0, 2 * M_PI, 0, 2 * M_PI, M, space_grey);

		// Build the lower ring
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_ring_xyz, 0, 2 * M_PI, 0, 2 * M_PI, M, space_grey);

		double beam_radius = 0.1;
		// Build the upper cross-bar 1
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the upper cross-bar 2
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the lower cross-bar 1
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the lower cross-bar 2
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the central axis
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.15;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_central_axis_xyz, 0, 2 * M_PI, -0.7, 0.7, M, space_grey);

		// Build the central axis upper cap
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(disk_xyz, 0, 0.2, 0, 2 * M_PI, M, space_grey);

		// Build the central axis lower cap
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(disk_xyz, 0, 0.3, 0, 2 * M_PI, M, space_grey);

		save_image(frame);
		frame++;
	}

	return 0;
}
