var a = [, , , , , , ];
exhaustiveSliceTest("exhaustive slice test 1", a);
function mySlice(a, from, to) {
    var to2 = to;
    if (to2 < 0) {
        to2 = to2.length + to;
    }
}
function exhaustiveSliceTest(testname, a) { x = a; }
for (y = a.length; y >= 0; y--) {
    mySlice(a, x, y);
}
