module a (reset, clk, z1);// z2, z3, z4);
input reset, clk;
output z1;// z2, z3, z4;

reg [1:0] x;
//reg [3:0]y;

//assign z1 = (x == 4'd200);
assign z1 = (x == 2'b10);
//assign z2 = !(x <= 2'd2);
//assign z3 = (x >= 2'd2);
//assign z4 = (x == 2'd2);// || (y == 4'd10);

always @(posedge clk) begin
   if (!reset) begin
      x <= 1'b0;
   //   y <= 4'd1;
   end
   else begin 
		x<=x+1;
   end
end
endmodule

