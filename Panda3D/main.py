from math import pi, sin, cos

from direct.showbase.ShowBase import ShowBase
from direct.task import Task
from direct.actor.Actor import Actor
from direct.interval.IntervalGlobal import Sequence
from panda3d.core import Point3


class App(ShowBase):
    def __init__(self):
        ShowBase.__init__(self)

        self.disable_mouse()

        self.scene = self.loader.loadModel("models/environment")
        self.scene.reparentTo(self.render)
        self.scene.setScale(0.25, 0.25, 0.25)
        self.scene.setPos(-8, 42, 0)

        self.taskMgr.add(self.spinCameraTask, "SpinCameraTask")

        self.pandaActor = Actor("models/panda-model", {"walk": "models/panda-walk4"})

        self.pandaActor.setScale(0.005, 0.005, 0.005)
        self.pandaActor.reparentTo(self.render)
        self.pandaActor.loop("walk")

        posInt1 = self.pandaActor.posInterval(
            13, Point3(0, -10, 0), startPos=Point3(0, 10, 0)
        )

        posInt2 = self.pandaActor.posInterval(
            13, Point3(0, 10, 0), startPos=Point3(0, -10, 0)
        )

        hprInt1 = self.pandaActor.hprInterval(
            3, Point3(180, 0, 0), startHpr=Point3(0, 0, 0)
        )
        hprInt2 = self.pandaActor.hprInterval(
            3, Point3(0, 0, 0), startHpr=Point3(180, 0, 0)
        )

        self.pandaPace = Sequence(posInt1, hprInt1, posInt2, hprInt2, name="PandaPace")

        self.pandaPace.loop()

    def spinCameraTask(self, task):
        angleDeg = task.time * 6.0
        angleRad = angleDeg * (pi / 180.0)
        self.camera.setPos(20 * sin(angleRad), -20 * cos(angleRad), 3)
        self.camera.setHpr(angleDeg, 0, 0)
        return Task.cont


app = App()
app.run()
