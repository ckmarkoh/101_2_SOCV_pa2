module bi(in,clock,x);
input in;
input clock;
wire g1,g2,g3,g4;
output [1:0] x;
wire [1:0] x;
wire [1:0] y;
reg [1:0] x_r;
assign g3= x[1] & (~in);
assign g4= x[0] & in;
assign g1= (~x[1]) & (~x[0]) & in;
assign g2= x[0] & (~in);
assign y[1]= g3|g4;
assign y[0]= g1|g2;
assign x=x_r;
always @(posedge clock) begin
	x_r<=y;
end

endmodule
