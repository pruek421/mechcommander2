//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#ifndef MLR_MLR_I_L_DET_TMESH_HPP
#define MLR_MLR_I_L_DET_TMESH_HPP

//#include <mlr/mlr.hpp>
//#include <mlr/mlr_i_c_det_tmesh.hpp>
//#include <mlr/mlr_i_l_det_pmesh.hpp>

namespace MidLevelRenderer {

	//##########################################################################
	//###### MLRIndexedTriMesh with color and lighting and detail texture ######
	//##########################################################################


	class MLR_I_L_DeT_TMesh:
		public MLR_I_C_DeT_TMesh
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Initialization
		//
	public:
		static void __stdcall InitializeClass(void);
		static void __stdcall TerminateClass(void);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Constructors/Destructors
		//
	protected:
		MLR_I_L_DeT_TMesh(
			ClassData *class_data,
			Stuff::MemoryStream *stream,
			int version
			);
		~MLR_I_L_DeT_TMesh();

	public:
		MLR_I_L_DeT_TMesh(ClassData *class_data=MLR_I_L_DeT_TMesh::DefaultData);

		static MLR_I_L_DeT_TMesh*
			Make(
			Stuff::MemoryStream *stream,
			int version
			);

		void
			Save(Stuff::MemoryStream *stream);

	public:
		virtual void
			SetNormalData(
			const Stuff::Vector3D *array,
			int point_count
			);
		virtual void
			GetNormalData(
			Stuff::Vector3D **array,
			int *point_count
			);

#if COLOR_AS_DWORD
		virtual void
			SetColorData(
			const ULONG *array,
			int point_count
			);
#else
		virtual void
			SetColorData(
			const Stuff::RGBAColor *array,
			int point_count
			);
#endif

		virtual void	Lighting(MLRLight* const*, int nrLights);

		virtual void
#if COLOR_AS_DWORD
			PaintMe(const ULONG *paintMe);
#else
			PaintMe(const Stuff::RGBAColor *paintMe);
#endif

		virtual int	
			TransformAndClip(Stuff::Matrix4D *, MLRClippingState, GOSVertexPool*,bool=false);

		virtual void
			TransformNoClip(Stuff::Matrix4D*, GOSVertexPool*,bool=false);

		void
			Copy(
			MLR_I_L_TMesh*,
			MLRState detailState,
			float xOff,
			float yOff,
			float xFac,
			float yFac
			);

		bool
			Copy(MLR_I_L_DeT_PMesh*);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Class Data Support
		//
	public:
		static ClassData* DefaultData;

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Testing
		//
	public:
		void TestInstance(void) const;

		virtual int
			GetSize()
		{ 
			Check_Object(this);
			int ret = MLR_I_C_DeT_TMesh::GetSize();
			ret += normals.GetSize();
			ret += litColors.GetSize();

			return ret;
		}

	protected:
		Stuff::DynamicArrayOf<Stuff::Vector3D> normals;		// Base address of normal list 

#if COLOR_AS_DWORD
		Stuff::DynamicArrayOf<ULONG> litColors;
#else
		Stuff::DynamicArrayOf<Stuff::RGBAColor> litColors;
#endif
	};

	//	MLR_I_L_DeT_TMesh*
	//		CreateIndexedTriCube_NoColor_NoLit(float, MLRState*);
	MLRShape*
		CreateIndexedTriIcosahedron_Color_Lit_DetTex(
		IcoInfo&,
		MLRState*,
		MLRState*
		);

}
#endif
