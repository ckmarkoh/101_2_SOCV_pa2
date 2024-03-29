module a (reset, clk,add, z1);// z2, z3, z4);
input reset, clk;
output z1;// z2, z3, z4;

reg [3:0]x;
//reg [3:0]y;

input [3:0] add;
//assign z1 = (x == 4'd200);
assign z1 = (x >= 4'b0100);
//assign z2 = !(x <= 2'd2);
//assign z3 = (x >= 2'd2);
//assign z4 = (x == 2'd2);// || (y == 4'd10);

always @(posedge clk) begin
   if (!reset) begin
      x <= 4'b0000;
   //   y <= 4'd1;
   end
   else	if(add==(x+4'b1))begin
		x<=x+add;
	end
end
endmodule

