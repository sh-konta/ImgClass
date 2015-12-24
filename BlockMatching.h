#ifndef LIB_ImgClass_BlockMatching
#define LIB_ImgClass_BlockMatching

/* Macro for compatibility where the C++11 is not supported
#ifndef nullptr
#define nullptr 0
#endif
*/


#include <list>
#include <vector>

#include "ImgClass.h"
#include "Lab.h"
#include "RGB.h"
#include "Vector.h"


template <class T>
class BlockMatching
{
	private:
		int _width;
		int _height;
		int _block_size;
		int _cells_width;
		int _cells_height;
		ImgVector<T> _image_prev;
		ImgVector<T> _image_next;
		ImgVector<int> _region_map_prev;
		ImgVector<int> _region_map_next;
		ImgVector<T> _color_quantized_prev;
		ImgVector<T> _color_quantized_next;
		ImgVector<VECTOR_2D<double> > _motion_vector;
		// For arbitrary shaped block matching
		std::vector<std::list<VECTOR_2D<int> > > _connected_regions_prev;
		std::vector<std::list<VECTOR_2D<int> > > _connected_regions_next;

	public:
		// Constructors
		BlockMatching(void);
		explicit BlockMatching(const BlockMatching& copy);
		BlockMatching(const ImgVector<T>& image_prev, const ImgVector<T>& image_next, const int BlockSize, const bool dense = false);
		BlockMatching(const ImgVector<T>& image_prev, const ImgVector<T>& image_next, const ImgVector<int>& region_prev, const ImgVector<int>& region_next);
		// Destructor
		virtual ~BlockMatching(void);

		// Resetter
		void reset(const ImgVector<T>& image_prev, const ImgVector<T>& image_next, const int BlockSize, const bool dense = false);
		void reset(const ImgVector<T>& image_prev, const ImgVector<T>& image_next, const ImgVector<int>& region_prev, const ImgVector<int>& region_next);

		// Get state
		int width(void) const;
		int height(void) const;
		int block_size(void) const;
		int vector_field_width(void) const;
		int vector_field_height(void) const;
		bool isNULL(void) const;

		// Get reference
		ImgVector<VECTOR_2D<double> >& ref_motion_vector(void);
		VECTOR_2D<double>& operator[](int n);
		VECTOR_2D<double>& at(int x, int y);
		VECTOR_2D<double>& at_block(int x, int y);

		// Get data
		// returns const to avoid to mistake get() for at()
		const VECTOR_2D<double> get(int x, int y); // NOT const method because it will make new motion vector when it haven't done block matching
		const VECTOR_2D<double> get_block(int x, int y); // NOT const method because it will make new motion vector when it haven't done block matching

		// Block Matching methods
		// Search in the range of [-floor(search_range / 2), floor(search_range / 2)]
		void block_matching(const int search_range = 41);

	protected:
		void image_normalizer(void);
		// Extract connected region from region_map
		void get_connected_region_list(std::vector<std::list<VECTOR_2D<int> > >* connected_regions, const ImgVector<int>& region_map);
		void get_color_quantized_image(ImgVector<T>* decreased_color_image, const ImgVector<T>& image, const std::vector<std::list<VECTOR_2D<int> > >& connected_regions);

		// Main method of block_matching
		void block_matching_lattice(const int search_range);
		void block_matching_dense_lattice(const int search_range);
		void block_matching_arbitrary_shaped(const int search_range);
		// Interpolate skipped Motion Vectors
		void vector_interpolation(const std::list<VECTOR_2D<int> >& flat_blocks, ImgVector<bool>* estimated);
		double MAD(const int x_l, const int y_l, const int x_r, const int y_r, const int block_width, const int block_height, const ImgVector<T>& limage, const ImgVector<T>& rimage);
		// Get gradients
		ImgVector<VECTOR_2D<double> >* grad_prev(const int top_left_x, const int top_left_y, const int crop_width, const int crop_height);

		// Correlation function
		double SAD(const int x_prev, const int y_prev, const int x_next, const int y_next, const int block_width, const int block_height);
		double MAD(const int x_prev, const int y_prev, const int x_next, const int y_next, const int block_width, const int block_height);
		double ZNCC(const int x_prev, const int y_prev, const int x_next, const int y_next, const int block_width, const int block_height);
		// Center color weighted
		double MAD_centered(const int x_prev, const int y_prev, const int x_next, const int y_next, const int block_width, const int block_height);
		// Arbitrary shaped correlation function
		double MAD_region(const int x_diff_prev, const int y_diff_prev, const std::list<VECTOR_2D<int> >& region);
		double ZNCC_region(const int x_diff_prev, const int y_diff_prev, const std::list<VECTOR_2D<int> >& region);
		double MAD_region_nearest_intensity(const int x_diff_prev, const int y_diff_prev, const std::list<VECTOR_2D<int> >& region);
		double ZNCC_region_nearest_intensity(const int x_diff_prev, const int y_diff_prev, const std::list<VECTOR_2D<int> >& region);
};

double norm(const double& value);

#include "BlockMatching_private_initializer.h"
#include "BlockMatching_private_accessor.h"
#include "BlockMatching_private_main.h"

#endif

