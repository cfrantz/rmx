
uniform int   scene_op;

float fBoolOps(vec3 p) {
	float box = fBox(p,vec3(1));
	float sphere = length(p-vec3(1))-1;
	float d;
	float r = 0.3;
	float n = 4;
	
	switch (scene_op) {
		case 0: d = min(box,sphere); break;
		case 1: d = max(box,sphere); break;
		case 2: d = max(box,-sphere); break;
		
		case 3: d = fOpUnionRound(box,sphere,r); break;
		case 4: d = fOpIntersectionRound(box,sphere,r); break;
		case 5: d = fOpDifferenceRound(box,sphere,r); break;
		
		case 6: d = fOpUnionChamfer(box,sphere,r); break;
		case 7: d = fOpIntersectionChamfer(box,sphere,r); break;
		case 8: d = fOpDifferenceChamfer(box,sphere,r); break;
		
		case 9 : d = fOpUnionColumns(box,sphere,r,n); break;
		case 10: d = fOpIntersectionColumns(box,sphere,r,n); break;
		case 11: d = fOpDifferenceColumns(box,sphere,r,n); break;
		
		case 12: d = fOpUnionStairs(box,sphere,r,n); break;
		case 13: d = fOpIntersectionStairs(box,sphere,r,n); break;
		case 14: d = fOpDifferenceStairs(box,sphere,r,n); break;
		
		case 15: d = fOpPipe(box,sphere,r*0.3); break;
		case 16: d = fOpEngrave(box,sphere,r*0.3); break;
		case 17: d = fOpGroove(box,sphere,r*0.3, r*0.3); break;
		case 18: d = fOpTongue(box,sphere,r*0.3, r*0.3); break;
	}
	
	return d;
}
