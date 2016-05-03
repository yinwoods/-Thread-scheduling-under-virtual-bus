import os
from datetime import datetime
from flask import Flask, Markup, render_template, session, redirect, url_for, flash
from flask_bootstrap import Bootstrap
from flask_wtf import Form
from wtforms import IntegerField, StringField, SubmitField
from wtforms.validators import Required
from flask_debugtoolbar import DebugToolbarExtension
from flask.ext.misaka import Misaka

app = Flask(__name__)

#支持bootstrap
bootstrap = Bootstrap(app)

#支持markdown解析
Misaka(app)

#从config.py中读取配置
app.config.from_object('config')

#支持调试工具
toolbar = DebugToolbarExtension(app)

#首页
@app.route('/')
def index():
	return render_template('index.html')

#毕设演示
@app.route('/show_bs', methods=['GET', 'POST'])
def show_bs(name=None):

	doc_json = []
	
	device_num = None
	global_status = None
	cpu_time = None
	damage_device = None

	cpuForm = CpuForm()
	if cpuForm.validate_on_submit():
		session['cpu_time'] = cpuForm.cpu_time.data
		device_num = session.get('device_num')
		global_status = session.get('global_status')
		damage_device = session.get('damage_device')

	controlForm = ControlForm()
	if controlForm.validate_on_submit():
		session['damage_device'] = controlForm.damage_device.data

		device_num = session.get('device_num')
		global_status = session.get('global_status')
		damage_device = session.get('damage_device')
		
		tmpLists = damage_device.split(',')
		for tmp in tmpLists:
			if int(tmp) >= device_num or int(tmp) < 0:
				flash('输入不符合规范，请重新输入！')
				return redirect(url_for('show_bs', name='show_bs'))

		os.system('exec/control.sh %s %s %s' % (device_num, global_status, damage_device))

		arr = global_status.split(',')
		indexList = damage_device.split(',')
		for index in indexList:
			arr[int(index)] = '3'
		global_status = ','.join(arr)
		session['global_status'] = global_status

	mountForm = MountForm()
	if mountForm.validate_on_submit():
		session['device_num'] = mountForm.device_num.data
		session['global_status'] = mountForm.global_status.data

		device_num = session.get('device_num')
		if device_num is not None and device_num not in range(0, 11):
			flash('请输入0-10之间的整数!')
			return redirect(url_for('show_bs', name='show_bs'))
		
		global_status = session.get('global_status')
		if global_status is not None and not len(global_status.split(',')) == device_num:
		 	flash('请输入%s个状态位并以逗号分开!' % device_num)
		 	return redirect(url_for('show_bs', name='show_bs'))

		os.system('exec/mount.sh %s %s' % (device_num, global_status))

	if device_num:
		for i in range(0, device_num):
			newline = []
			for line in open('/dev/memdev%s' % i):
				results = line.split('：')
				if len(results) == 2:
					newline += results[1].lstrip().rstrip()
			doc_json += [newline]
	
	return render_template('show_bs.html', mountForm=mountForm, cpuForm=cpuForm, controlForm=controlForm, device_num=session.get('device_num'), global_status=session.get("global_status"), cpu_time=session.get('cpu_time'), doc_json=doc_json)

#关于毕设
@app.route('/bs_introduce')
def bs_introduce():
	return render_template('bs_introduce.html')

#毕设进度
@app.route('/bs_schedule')
def bs_schedule():
	content = ""
	with open("README.md", "r") as readme:
		content = readme.read()
	return render_template('bs_schedule.html', content=content)

#关于我
@app.route('/about_me')
def about_me():
	return render_template('about_me.html')

#404页面
@app.errorhandler(404)
def page_not_found(e):
	return render_template('404.html'), 404

#挂载驱动设备表单
class MountForm(Form):
	device_num = IntegerField(label='输入要创建设备个数：', validators=[Required()])
	global_status = StringField(label='输入状态位：', validators=[Required()])
	submit = SubmitField(label='演示项目')

#模拟控制表单
class ControlForm(Form):
	damage_device = StringField(label='输入要模拟损坏的设备编号：', validators=[Required()])
	submit = SubmitField(label='开始模拟')

#CPU限制表单
class CpuForm(Form):
	cpu_time = IntegerField(label="输入想要限制的CPU比例：", validators=[Required()])
	submit = SubmitField(label="限制CPU")

if __name__ == '__main__':
    app.run(host='0.0.0.0')
