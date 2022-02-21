#include <SFML/Graphics.hpp>

namespace Street 
{
	class Streets
	{
	public:
		int start_x;
		int start_y;
		int end_x;
		int end_y;

		void set_start(int x, int y) {
			start_x = x;
			start_y = y;
		}

		void set_end(int x, int y) {
			end_x = x;
			end_y = y;
		}
	};
}

