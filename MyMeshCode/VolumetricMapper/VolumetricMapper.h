#pragma once
#include <MeshLib/core/Geometry/Point.h>

namespace MeshLib {
		struct TMeshLib::VolumetricMapperCore;
		class CVolumetricMapper {
		public:
			CVolumetricMapper();
			void readSourceTMesh(const char *);
			void readTargetTMesh(const char *);

			CPoint map(CPoint p);
			CPoint reverseMap(CPoint p);

		private:
			VolumetricMapperCore core;

		};
}