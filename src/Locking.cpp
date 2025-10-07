// HEADER ONLY :)
// in this file I just wanted to defend my choice of using an AVL tree 
// over other data structures like hash maps or B-trees for implementing a sorted byte tree.
// I chose an AVL tree because it provides efficient O(log n) time complexity for insertion, deletion, and lookup operations,
// while maintaining a balanced structure. This balance ensures that the tree remains efficient even in the worst case 
// scenarios, unlike unbalanced binary search trees which can degrade to O(n) time complexity.
// Since I like to keep thigs fast I chose an AVL Tree 

/*
here is some shader code just because I like shaders :D
for shader toy 
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,1,9));

    fragColor = vec4(col,1.0);
}






*/