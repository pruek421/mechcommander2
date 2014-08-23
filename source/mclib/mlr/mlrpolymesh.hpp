//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#ifndef MLR_MLRPOLYMESH_HPP
#define MLR_MLRPOLYMESH_HPP

//#include <mlr/mlr.hpp>
//#include <mlr/mlrprimitive.hpp>

namespace MidLevelRenderer {

	//##########################################################################
	//########################    MLRPolyMesh    ###############################
	//##########################################################################


	class MLRPolyMesh:
		public MLRPrimitive
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
		MLRPolyMesh(
			Stuff::MemoryStream *stream,
			int version
			);
		~MLRPolyMesh(void);

	public:
		MLRPolyMesh(void);

		static MLRPolyMesh*
			Make(
			Stuff::MemoryStream *stream,
			int version
			);

		void
			Save(Stuff::MemoryStream *stream);

		virtual void	SetPrimitiveLength(puint8_t , int);
		virtual void	GetPrimitiveLength(puint8_t *, int*);

		void	FindFacePlanes(void);

		virtual int	FindBackFace(const Stuff::Point3D&);

		const Stuff::Plane *GetPolygonPlane(int i)
		{
			Check_Object(this);
			Verify(i<facePlanes.GetLength(void));

			return &facePlanes[i];
		}

		virtual void	Lighting(MLRLight**, int nrLights);

		virtual int	Clip(MLRClippingState, GOSVertexPool*);

		void
			Transform(Stuff::Matrix4D*);

		virtual void
			TransformNoClip(Stuff::Matrix4D*, GOSVertexPool*);

		//	Initializes the visibility test list
		void
			ResetTestList(void);

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
			GetSize(void)
		{ 
			Check_Object(this);
			int ret = MLRPrimitive::GetSize(void);
			ret += testList.GetSize(void);
			ret += facePlanes.GetSize(void);

			return ret;
		}

	protected:
		Stuff::DynamicArrayOf<uint8_t>	testList;

		Stuff::DynamicArrayOf<Stuff::Plane> facePlanes;

	};

	MLRPolyMesh*
		CreateCube(float, Stuff::RGBAColor*, Stuff::Vector3D*, MLRState*);

}
#endif
