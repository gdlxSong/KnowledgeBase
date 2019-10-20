# encoding = utf-8


import os
import re



head = """
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Gdl.Music-v2.0</title>
	<style type="text/css">
		/*实现背景图片透明，内容不透明*/

		html, body {
			overflow: hidden;
			margin: 0;
			padding: 0;
			width: 100%;
			height: 100%;
			background-color: rgba(0,0,0,0.2)
			touch-action: none;
			content-zooming: none;
			position: absolute;
		}
		#plot{
			margin-top: 0%;
			z-index: 5;
			position:absolute;
		}

		#music {
			width: 500px;
			height: 500px;
			border-radius: 10px;
			margin: 20px auto;
			position: relative;
			left: 250px;
			top:250px;
			background: url(./image/一月，银装轻舞-紫竹笛韵.jpg) no-repeat;
			background-size: cover;
			text-align: center;
			border-radius: 80px 80px 50px 50px; 
			overflow: hidden;
		}
		#container {
			position: absolute;
			left: 0;
		    right: 0;
		    top: 0;
		    bottom: 0;
		    width: 500px;
		    height: 500px;
		    text-align: center;
		    background:rgba(255,255,255,0.6);
		}
		/*实现背景图片透明，内容不透明*/

		#musicImg {
			width: 280px;
			height: 280px;
			border-radius: 50%;
		}
		audio {
			display: block;
			margin: 20px auto;
		}
		#musicName {
			margin: 1% 20%;
			padding-top: 10px;
			line-height: 30px;
			color: #cc1b1b;
		}
		.btn>button {
			width: 60px;
			height: 30px;
			border-radius: 10px;
			background: skyblue;
		}

		canvas {
			position: relative;
			width: 100%;
			height: 100%;
			background:#000;
			cursor: pointer;
		}

		#footer{
			position: fixed;
			left: 5%;
			bottom: 1em;
			width: 90%;
			height: 2em;
		}
		a{
			text-decoration:none;
		}
	</style>
</head>
<body >

	<div id="plot">

		<div id="music">
			<div id="container">
				<h3 id="musicName" align="center">一月，银装轻舞-紫竹笛韵</h3>
				<img src="image/一月，银装轻舞-紫竹笛韵.jpg" id="musicImg">
				<audio src="./music/一月，银装轻舞-紫竹笛韵.mp3" controls id="audio"></audio>
				<div class="btn">
					<button id="play">play</button>
					<button id="pause">pause</button>
					<button id="prev">prev</button>
					<button id="next">next</button>
				</div>
			</div>
		</div>
		<script type="text/javascript">
			
			var play = document.getElementById('play'),
				pause = document.getElementById('pause'),
				prev = document.getElementById('prev'),
				next = document.getElementById('next'),
				musicName = document.getElementById('musicName'),
				musicImg = document.getElementById('musicImg'),
				bgImage = document.getElementById('music');


			//initialize bg_index...
			var bg_index = 0;
			//上一次的播放时间.
			var pre_time = 0;
			//audio 的就绪状态. 0:未就绪， 4：已就绪
			var flag_once = true;
			//timer....
			var g_timer = setTimeout(function(){//donothing...
				}, 0);
"""
session = """
			var music_bg = new Array();
			music_bg = {backgrounds};
            var mlen = music_bg.length;
			


			//song...
			var songs = new Array();
			songs = {songs};
			var len = songs.length;
			var num = Math.floor(Math.random()*len);
"""
tail = """
//-----------------居中..........
			//居中..
			lft = window.innerWidth / 2 - 250;
			bgImage.style.left = lft + "px";
			top0 = window.innerHeight / 2 - 250;
			bgImage.style.top = top0 + "px";

			// music居中， 随窗口大小刷新.
			window.onresize =  function(){
				bgImage = document.getElementById('music');
				lft = window.innerWidth / 2 - 250;
				bgImage.style.left = lft + "px";
				top0 = window.innerHeight / 2 - 250;
				bgImage.style.top = top0 + "px";
			}



			//获取一个与输入值不同的索引值.
			function getIndex(index_pre){
				var index = 0;
				while(true){
					index = Math.floor(Math.random() * mlen);
					if(index != index_pre)
						break;
				}
				return index;
			}

			//获取当前播放位置/秒.
			function getCurrentSeekTime(){

				//获取audio对象. audio.src
				return document.getElementById("audio").currentTime;
			}

			//监测5秒的时间内音乐的播放进度是否改变，如果没有，那么播放下一首....
			function checkAndChange_per_5_seconds(){
				

				
				if(document.getElementById("audio").paused == false){
					flag_once = true;
					if(Math.floor(pre_time) == Math.floor(getCurrentSeekTime())){
						//pre_time == cur_t, 切歌.
						next.click();
					}
					else
						//update pre_time.
						pre_time = getCurrentSeekTime();
				}
				else{
					//检测audio是否就绪..
					if(0 == document.getElementById("audio").readyState){
						//readyState == 0, 未就绪.
						if(flag_once)//第一次进入函数...
							flag_once = false;
						else{
							//直接切歌...
							next.click();
							flag_once = true;
						}
					}
				}
				//创建定时器，5s
				g_timer = setTimeout(checkAndChange_per_5_seconds, 10000);
			}
			//启动..
			checkAndChange_per_5_seconds();


			//reset timer...
			function resetTimer(){
				//clear timer..
				clearTimeout(g_timer);
				//reset..
				g_timer = setTimeout(checkAndChange_per_5_seconds, 10000);
				pre_time = 0;
			}

			// 播放
			play.onclick = function(){
				if(audio.paused){
					audio.play();
				}
				//reset timer...
				resetTimer();
				
			}

			// 暂停
			pause.onclick = function(){
				if(audio.played){
					audio.pause();
				}
				//clear timer...
				if(document.getElementById("audio").paused == false)
					clearTimeout(g_timer);
			}

			// 上一首
			prev.onclick = function(){

				bg_index = getIndex(bg_index);		//0 ~ len-1
				num = (num + len - 1) % len;
				audio.src = './music/' + songs[num] + '.mp3';
				musicName.innerHTML = songs[num];
				bgImage.style.backgroundImage = 'url(./image/' + music_bg[bg_index] + '.jpg)';
				musicImg.src = './image/' + music_bg[bg_index] + '.jpg';
				audio.play();
				resetTimer();
			}

			// 下一首
			next.onclick = function(){

				bg_index = getIndex(bg_index);		//0 ~ len-1
				num = (num + 1) % len;
				audio.src = './music/' + songs[num] + '.mp3';
				musicName.innerHTML = songs[num];
				bgImage.style.backgroundImage = 'url(./image/' + music_bg[bg_index] + '.jpg)';
				musicImg.src = './image/' + music_bg[bg_index] + '.jpg';
				audio.play();
				resetTimer();
			}

			// 自动切换下一首
			audio.addEventListener('ended',function(){
				next.onclick();
			},false);

			//自动执行开始切一首歌，
			next.click();
		</script>
	</div>
<script>
"use strict";
{
	const perlin = {
		init () {
			this.p = new Uint8Array(512);
			this.reset();
		},
		reset() {
			for (let i = 0; i < 512; ++i) {
				this.p[i] = Math.random() * 256;
			}
		},
		lerp(t, a, b) {
			return a + t * (b - a);
		},
		grad2d(i, x, y) {
			const v = (i & 1) === 0 ? x : y;
			return (i & 2) === 0 ? -v : v;
		},
		noise2d(x2d, y2d) {
			const X = Math.floor(x2d) & 255;
			const Y = Math.floor(y2d) & 255;
			const x = x2d - Math.floor(x2d);
			const y = y2d - Math.floor(y2d);
			const fx = (3 - 2 * x) * x * x;
			const fy = (3 - 2 * y) * y * y;
			const p0 = this.p[X] + Y;
			const p1 = this.p[X + 1] + Y;
			return this.lerp(
				fy,
				this.lerp(
					fx,
					this.grad2d(this.p[p0], x, y),
					this.grad2d(this.p[p1], x - 1, y)
				),
				this.lerp(
					fx,
					this.grad2d(this.p[p0 + 1], x, y - 1),
					this.grad2d(this.p[p1 + 1], x - 1, y - 1)
				)
			);
		}
	};
/////////////////////////////////////////////////////////////////
	const canvas = {
		init() {
			this.elem = document.createElement("canvas");
			document.body.appendChild(this.elem);
			this.width = this.elem.width = this.elem.offsetWidth;
			this.height = this.elem.height = this.elem.offsetHeight;
			return this.elem.getContext("2d");
		}
	};
/////////////////////////////////////////////////////////////////
	const webgl = {
		init(canvas, options) {
			this.elem = document.createElement("canvas");
			this.gl = (
				this.elem.getContext("webgl", options) ||
				this.elem.getContext("experimental-webgl", options)
			);
			if (!this.gl) return false;
			const vertexShader = this.gl.createShader(this.gl.VERTEX_SHADER);
			this.gl.shaderSource(vertexShader, `
				precision highp float;
				attribute vec3 aPosition;
				uniform vec2 uResolution;
				void main() {
					gl_PointSize = 1.0;
					gl_Position = vec4(
						( aPosition.x / uResolution.x * 2.0) - 1.0, 
						(-aPosition.y / uResolution.y * 2.0) + 1.0, 
						0.0,
						1.0
					);
				}`
			);
			this.gl.compileShader(vertexShader);
			const fragmentShader = this.gl.createShader(this.gl.FRAGMENT_SHADER);
			this.gl.shaderSource(fragmentShader, `
				precision highp float;
				void main() {
					gl_FragColor = vec4(0.2, 0.3, 1.0, 1.0);
				}`
			);
			this.gl.compileShader(fragmentShader);
			const program = this.gl.createProgram();
			this.gl.attachShader(program, vertexShader);
			this.gl.attachShader(program, fragmentShader);
			this.gl.linkProgram(program);
			this.gl.useProgram(program);
			this.aPosition = this.gl.getAttribLocation(program, "aPosition");
			this.gl.enableVertexAttribArray(this.aPosition);
			this.positionBuffer = this.gl.createBuffer();
			this.elem.width = canvas.width;
			this.elem.height = canvas.height;
			const uResolution = this.gl.getUniformLocation(program, "uResolution");
			this.gl.enableVertexAttribArray(uResolution);
			this.gl.uniform2f(uResolution, canvas.width, canvas.height);
			this.gl.viewport(
				0,
				0,
				this.gl.drawingBufferWidth,
				this.gl.drawingBufferHeight
			);
			return this.gl;
		},
		drawBuffer(data, num) {
			this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.positionBuffer);
			this.gl.vertexAttribPointer(this.aPosition, 2, this.gl.FLOAT, false, 0, 0);
			this.gl.bufferData(
				this.gl.ARRAY_BUFFER,
				data,
				this.gl.DYNAMIC_DRAW
			);
			this.gl.drawArrays(this.gl.GL_POINTS, 0, num);
		}
	};
/////////////////////////////////////////////////////////////////
	const ctx = canvas.init();
	const gl = webgl.init(canvas, {
		alpha: false,
		stencil: false,
		antialias: false,
		depth: false,
	});
	perlin.init();
	const nParticles = 1000; 
	const velocities = new Float32Array(nParticles * 2);
	const particles = new Float32Array(nParticles * 2);
	let frame = 0;
	for (let i = 0; i < nParticles; i++) {
		const p = i * 2;
		particles[p+0] = Math.random() * canvas.width;
		particles[p+1] = Math.random() * canvas.height;
	}
/////////////////////////////////////////////////////////////////
	const run = () => {
		requestAnimationFrame(run);
		frame++;
		gl.clear(gl.COLOR_BUFFER_BIT);
		ctx.globalCompositeOperation = "source-over";
		ctx.fillStyle = "rgba(0, 0, 0, 0.025)";
		ctx.fillRect(0, 0, canvas.width, canvas.height);
		ctx.globalCompositeOperation = "lighter";
		for (let i = 0; i < nParticles; i++) {
			const p = i * 2;
			let n = 80 * perlin.noise2d(particles[p+0] * 0.001, particles[p+1] * 0.001);
			velocities[p+0] += 0.1 * Math.cos(n);
			velocities[p+1] += 0.1 * Math.sin(n);
			particles[p+0]  += (velocities[p+0] *= 0.99);
			particles[p+1]  += (velocities[p+1] *= 0.99);
			particles[p+0]   = (canvas.width  + particles[p+0]) % canvas.width;
			particles[p+1]   = (canvas.height + particles[p+1]) % canvas.height;
		}
		webgl.drawBuffer(particles, nParticles);
		if (frame > 30) ctx.drawImage(webgl.elem, 0, 0);	
	};
	requestAnimationFrame(run);
/////////////////////////////////////////////////////////////////
	["click", "touchdown"].forEach(event => {
		document.addEventListener(event, e => perlin.reset(), false);
	});
}
</script>

<div id="footer">
<marquee id="marq"behavior="alternate" truespeed=true><a href="http://www.yqun.xyz/gdl/Music/v1.0/">Gdl.Musicplayer.v2.0 is the latest version, the old version: Gdl.Musicplayer.v1.0</a></marquee>
</div>
</body>
</html>"""



# lalalla~ 正式开始自动生成列表了...

if __name__ == "__main__":

    outpath = "./"
    # 读取歌曲列表...
    mdirs = os.listdir(r"./music")
    picdirs = os.listdir(r"./image")


    for index, x in enumerate(mdirs):
        mdirs[index] = re.sub("\..*$", "", x, count=1, flags=0)# x.rstrip(".mp3")
    # print(mdirs)
    for index, x in enumerate(picdirs):
        picdirs[index] = re.sub("\..*$", "", x, count=1, flags=0)# x.rstrip(".jpg")
    # print(picdirs)
    session = session.format(backgrounds = repr(picdirs), songs = repr(mdirs))
    
    with open(outpath+"index.html", "w+", encoding="utf8") as file:
        file.write(head+session+tail)




