// I should at some point go through this and fix all the naming convention stuff
// but i cannot be bothered to rn
namespace render
{
    // ok now lets do the
    // thinking
    // To render we have to copy the data
    // from several 'textures'/maps to the gpu
    // and then it just does the math required
    // to turn that into the final product
    // So what I need is a function which
    // just fetches the data from the cell data
    // type, and from the rgb data type
    // Also needs to just edit a specific rang ofc
    //
    // We'll also need to leave the alpha channel open
    // to being changed, cause like compositing sanity

}
