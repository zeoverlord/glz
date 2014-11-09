//
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

// while it is not required i like to request a few things
// 1. please do share any meaningfull/usefull changes/additions/fixes you make with me so that i could include it in any future version
// 2. likewise do share any ideas for improvements
// 3. If you make something comersiol or at least something you release publicly that relies on this code then i would like to know and maybe use in my CV
// 4. Please do include me in your credits

// glz geometry toolkit 2d- for allmost all your 2d geometry needs
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


using namespace std;

#include "ztool-type.h"
#include "ztool-geo.h"
#include "ztool-vectormath.h"

#ifndef __glz_geogenerate__
#define __glz_geogenerate__

long glzVAOMakeFromFile(string const filename, glzMatrix matrix, texture_transform tt, unsigned int *vao);
long glzVAOMakeFromFileVector(string const filename, glzMatrix matrix, texture_transform tt, unsigned int *vao);
long glzVAOMakeFromVector(vector<poly3> pdata, unsigned int *vao);

long glzVAOMakeAtlasGrid(unsigned int x, unsigned int y, glzMatrix matrix, texture_transform tt, unsigned int *vao);
long glzVAOMakeHeightAtlasGrid(unsigned int x, unsigned int y, glzMatrix matrix, texture_transform tt, image_geo_transform igt, unsigned int *vao);

long glzVAOCountPrimitiveVerts(float varation, float resuloution, unsigned int primitive, texture_transform tt);
long glzVAOMakePrimitive(primitive_gen pg, unsigned int *vao);
long glzVAOMakePrimitives(int num, primitive_gen pg[], unsigned int *vao);


#endif /* __glz_geogenerate__ */